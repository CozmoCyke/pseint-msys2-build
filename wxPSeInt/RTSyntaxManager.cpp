#include <iostream>
#include <wx/txtstrm.h>
#include "RTSyntaxManager.h"
#include "mxProcess.h"
#include "ConfigManager.h"
#include "mxMainWindow.h"
#include "mxVarWindow.h"
#include "ids.h"
#include "Logger.h"
#include "string_conversions.h"

RTSyntaxManager *RTSyntaxManager::the_one = nullptr;

RTSyntaxManager::RTSyntaxManager():wxProcess(wxPROCESS_REDIRECT),conv("ISO-8851") {
	_LOG("RTSyntaxManager::RTSyntaxManager this="<<this);
	_LOG("RTSyntaxManager::RTSyntaxManager MARKER_123");
	processing=running=restart=false;
	timer = new wxTimer(main_window->GetEventHandler(),mxID_RT_TIMER);
}
void RTSyntaxManager::BeginVarsInputIfNeeded() {
	if (m_vars_input_started || !vars_window) return;
	_LOG("RTSyntaxManager::BeginVarsInputIfNeeded start pending_proc_valid="<<m_pending_proc_valid<<" pending_proc_name=["<<m_pending_proc_name<<"] main="<<m_pending_proc_main);
	vars_window->BeginInput();
	m_vars_input_started = true;
	if (m_pending_proc_valid) {
		_LOG("RTSyntaxManager::BeginVarsInputIfNeeded replay pending proc name=["<<m_pending_proc_name<<"] main="<<m_pending_proc_main);
		vars_window->AddProc(m_pending_proc_name, m_pending_proc_main);
		m_pending_proc_valid = false;
	}
}

void RTSyntaxManager::RunPendingCycle(const char *reason) {
	if (!m_has_pending || !m_pending_src) return;
	mxSource *pending_src = m_pending_src;
	std::function<void()> pending_action = std::move(m_pending_action_post);
	m_pending_src = nullptr;
	m_pending_action_post = {};
	m_has_pending = false;
	std::fprintf(stderr, "DBG RTS RUN PENDING reason=%s src=%p\n", reason, (void*)pending_src);
	std::fflush(stderr);
	RTSyntaxManager::Process(pending_src, std::move(pending_action));
}

void RTSyntaxManager::TimeoutRecycle(const char *phase, mxSource *src) {
	processing = false;
	m_current_step = Step::None;
	m_empty_reads = 0;
	restart = true;
	_LOG("RTSyntaxManager::ContinueProcessing TIMEOUT "<<phase<<" -> RESTART RT PROCESS src="<<src);
	std::fprintf(stderr, "DBG RTS TIMEOUT %s -> RESTART RT PROCESS src=%p pid=%ld\n",
		phase,
		(void*)src,
		(long)pid);
	std::fflush(stderr);
	Stop();
}

void RTSyntaxManager::Start ( ) {
	_LOG("RTSyntaxManager::Start ENTER the_one="<<the_one);
	_LOG("RTSyntaxManager::Start MARKER_456");
	if (the_one) {
		_LOG("RTSyntaxManager::Start existing the_one running="<<the_one->running<<" processing="<<the_one->processing<<" restart="<<the_one->restart<<" pid="<<the_one->pid);
		if (the_one->running) return;
	}
	if (!the_one) the_one=new RTSyntaxManager;
	the_one->restart=false;
	std::cerr
		<< "CFGTRACE where=RTSyntaxManager::Start"
		<< " profile_name=" << cfg_lang.name
		<< " source=" << cfg_lang.source
		<< " profile_bits=" << cfg_lang.GetAsSingleString()
		<< " overload_equal=" << (cfg_lang[LS_OVERLOAD_EQUAL] ? 1 : 0)
		<< std::endl;
	wxString command;
	command<<config->pseint_command<<" --forrealtimesyntax "<<mxProcess::GetProfileArgs();
	std::fprintf(stderr, "DBG RTS START command=[%s]\n", (const char*)_W2S(command));
std::fflush(stderr);
	the_one->pid=wxExecute(command,wxEXEC_ASYNC,the_one);
	the_one->running=true;
	_LOG("RTSyntaxManager::Start pid="<<the_one->pid<<" command="<<command<<" the_one="<<the_one);
}

void RTSyntaxManager::Stop ( ) {
	_LOG("RTSyntaxManager::Stop the_one="<<the_one);
	if (the_one && the_one->pid<=0) return;
	if (the_one) {
		_LOG("RTSyntaxManager::Stop killing pid="<<the_one->pid<<" src="<<the_one->src);
		the_one->Kill(the_one->pid,wxSIGKILL);
		the_one->src=NULL;
	}
}

void RTSyntaxManager::Restart ( ) {
	_LOG("RTSyntaxManager::Restart the_one="<<the_one);
	if (!the_one) Start();
	else {
		the_one->restart=true;
		_LOG("RTSyntaxManager::Restart set restart=true and stopping pid="<<the_one->pid);
		Stop();
	}
}

bool RTSyntaxManager::Process (mxSource *src, std::function<void()> &&action_post) {
	std::fprintf(stderr, "DBG RTS Process ENTER src=%p the_one=%p\n", (void*)src, (void*)the_one);
	std::fflush(stderr);

	if (!src) {
		if (the_one && the_one->processing) {
			std::fprintf(stderr, "DBG RTS Process src=NULL but processing=true, continuing\n");
			std::fflush(stderr);
			the_one->ContinueProcessing();
			return true;
		}
		std::fprintf(stderr, "DBG RTS Process ERROR: Process(NULL) with invalid state the_one=%p\n", (void*)the_one);
		std::fflush(stderr);
		return false;
	}

	if (!the_one) {
		std::fprintf(stderr, "DBG RTS Process no instance, calling Start()\n");
		std::fflush(stderr);
		Start();
	} else if (the_one->processing || the_one->restart) {
		the_one->m_pending_src = src;
		the_one->m_pending_action_post = std::move(action_post);
		the_one->m_has_pending = true;
		std::fprintf(stderr, "DBG RTS Process QUEUED pending cycle src=%p processing=%d restart=%d pid=%ld\n",
		             (void*)src, (int)the_one->processing, (int)the_one->restart, (long)the_one->pid);
		std::fflush(stderr);
		return true;
	}

	std::fprintf(stderr, "DBG RTS Process PREPARE src=%p lineCount=%d\n",
	             (void*)src, src->GetLineCount());
	std::fflush(stderr);

	the_one->src = src;
	the_one->m_action_post = action_post;
	the_one->m_current_step = Step::SendCode;
	the_one->m_vars_input_started = false;
	the_one->m_pending_proc_valid = false;
	the_one->m_pending_proc_main = false;
	the_one->m_pending_proc_name.Clear();
	the_one->m_empty_reads = 0;
	the_one->m_stderr_buf.clear();
	the_one->m_stdout_bytes = 0;

	wxOutputStream *raw_output = the_one->GetOutputStream();
	std::fprintf(stderr, "DBG RTS OUTPUT STREAM before_write raw=%p ok=%d\n",
		(void*)raw_output,
		raw_output && raw_output->IsOk() ? 1 : 0);
	std::fflush(stderr);
	wxTextOutputStream output(*raw_output
#ifdef UNICODE
							  ,wxEOL_NATIVE,the_one->conv
#endif
							  );

	the_one->processing = true;

	for (int i = 0; i < src->GetLineCount(); ++i) {
		wxString s = src->GetLine(i);
		src->ToRegularOpers(s);
		src->FixExtraUnicode(s);
		while ((not s.IsEmpty()) && (s.Last() == '\r' || s.Last() == '\n')) s.RemoveLast();

		std::fprintf(stderr, "DBG RTS SEND line[%d]=[%s]\n", i, (const char*)_W2S(s));
		std::fflush(stderr);

		output << s << "\n";
		wxMilliSleep(0);
	}

	std::fprintf(stderr, "DBG RTS SEND END_OF_INPUT\n");
	std::fflush(stderr);
	output << "<!{[END_OF_INPUT]}!>\n";
	std::fprintf(stderr, "DBG RTS OUTPUT STREAM after_write raw=%p ok=%d\n",
		(void*)raw_output,
		raw_output && raw_output->IsOk() ? 1 : 0);
	std::fflush(stderr);
	if (raw_output) {
		std::fprintf(stderr, "DBG RTS OUTPUT STREAM before_sync raw=%p ok=%d\n",
			(void*)raw_output,
			raw_output->IsOk() ? 1 : 0);
		std::fflush(stderr);
		raw_output->Sync();
		std::fprintf(stderr, "DBG RTS OUTPUT STREAM after_sync raw=%p ok=%d\n",
			(void*)raw_output,
			raw_output->IsOk() ? 1 : 0);
		std::fflush(stderr);
	}

	src->RTOuputStarts();
	the_one->m_current_step = Step::ReadErrors;

	std::fprintf(stderr, "DBG RTS Process BeginInput src=%p\n", (void*)src);
	std::fflush(stderr);
	////vars_window->BeginInput();

	std::fprintf(stderr, "DBG RTS Process calling ContinueProcessing current_step=ReadErrors\n");
	std::fflush(stderr);
	the_one->ContinueProcessing();

	std::fprintf(stderr, "DBG RTS Process EXIT src=%p processing=%d\n",
	             (void*)src, (int)the_one->processing);
	std::fflush(stderr);

	return true;
}
void RTSyntaxManager::ContinueProcessing() {
	_LOG("RTSyntaxManager::ContinueProcessing ENTER src="<<src<<" step="<<(int)m_current_step<<" processing="<<processing<<" pid="<<pid<<" inputAvailable="<<IsInputAvailable());
	wxTextInputStream input(*(GetInputStream()));	
	auto drain_stderr = [&]() {
		if (!IsErrorAvailable()) return;
		wxTextInputStream err(*(GetErrorStream()));
		const size_t kMaxBuf = 8 * 1024;
		while (IsErrorAvailable()) {
			char c = err.GetChar();
			m_stderr_buf.push_back(c);
			if (m_stderr_buf.size() > kMaxBuf) {
				m_stderr_buf.erase(0, m_stderr_buf.size() - kMaxBuf);
			}
		}
	};

	if (!src) {
		_LOG("RTSyntaxManager::ContinueProcessing src=NULL draining input");
		while (IsInputAvailable()) { /*char c=*/input.GetChar(); }
		return;
	}

	while(true) {
		if (!IsInputAvailable()) {
			drain_stderr();
			++m_empty_reads;
			_LOG("RTSyntaxManager::ContinueProcessing NO INPUT, scheduling timer src="<<src<<" step="<<(int)m_current_step<<" processing="<<processing<<" empty_reads="<<m_empty_reads);
			std::fprintf(stderr, "DBG RTS NOINPUT step=%d idle_reads=%d processing=%d src=%p\n",
				(int)m_current_step,
				m_empty_reads,
				(int)processing,
				(void*)src);
			std::fflush(stderr);
			if ((m_current_step == Step::ReadErrors || m_current_step == Step::ReadVars || m_current_step == Step::ReadBlocks)
				&& m_empty_reads >= 50) {
				const char *phase = (m_current_step == Step::ReadErrors) ? "ReadErrors"
				                   : (m_current_step == Step::ReadVars) ? "ReadVars"
				                   : "ReadBlocks";
				_LOG("RTSyntaxManager::ContinueProcessing RT TIMEOUT (no input) in "<<phase<<" src="<<src<<" empty_reads="<<m_empty_reads);
				std::fprintf(stderr, "DBG RTS TIMEOUT %s (no input) empty_reads=%d src=%p processing=%d\n",
					phase,
					m_empty_reads,
					(void*)src,
					(int)processing);
				std::fprintf(stderr, "DBG RTS TIMEOUT stdout_bytes=%zu\n", m_stdout_bytes);
				std::fprintf(stderr, "DBG RTS TIMEOUT stderr_buf_bytes=%zu\n", m_stderr_buf.size());
				if (!m_stderr_buf.empty()) {
					std::fprintf(stderr, "DBG RTS TIMEOUT stderr_tail=[%s]\n", m_stderr_buf.c_str());
				}
				std::fflush(stderr);
				if (src) src->RTOuputEnds();
				TimeoutRecycle(phase, src);
				return;
			}
			timer->Start(100,true);
			return;
		}
		static std::string aux_line;
		aux_line.clear();
		bool read_any = false;

		while (IsInputAvailable()) {
			char c=input.GetChar();
			++m_stdout_bytes;
			read_any = true;
			if (c=='\n') break;
			if (c!='\r') aux_line+=c;
		}
		if (!read_any) {
			timer->Start(100,true);
			return;
		}

		wxString line(_Z(aux_line.c_str()));
		std::fprintf(stderr, "DBG RTS line=[%s] step=%d\n", aux_line.c_str(), (int)m_current_step);
std::fflush(stderr);
		_LOG("RTSyntaxManager::ContinueProcessing RAW aux_line=["<<aux_line.c_str()<<"] line=["<<line<<"] step="<<(int)m_current_step<<" inputAvailableAfterRead="<<IsInputAvailable());

		if (not line.IsEmpty()) {
			m_empty_reads = 0;
			switch(m_current_step) {
				case Step::None:
					_LOG("RTSyntaxManager::ContinueProcessing ERROR Step::None");
					_impossible;
					break;

				case Step::SendCode:
					_LOG("RTSyntaxManager::ContinueProcessing ERROR Step::SendCode");
					_impossible;
					break;

				case Step::ReadErrors:
					_LOG("RTERRORS line=["<<line<<"]");
					if (line=="<!{[END_OF_OUTPUT]}!>") { 
						_LOG("RTSyntaxManager::ContinueProcessing phase ReadErrors->ReadVars src="<<src);
						m_current_step = Step::ReadVars;
					} else {
						if (config->rt_syntax) {
							_LOG("RTERRORS MarkError line=["<<line<<"]");
							src->MarkError(line);
						}
					}
					break;

    case Step::ReadVars:
    std::fprintf(stderr, "DBG RTVARS raw=[%s]\n", aux_line.c_str());
std::fflush(stderr);
        if (line=="<!{[END_OF_VARS]}!>") {
            _LOG("RTVARS END_OF_VARS src="<<src);
            if (m_vars_input_started && vars_window) vars_window->EndInput();
            m_vars_input_started = false;
            m_pending_proc_valid = false;
            m_pending_proc_main = false;
            m_pending_proc_name.Clear();
            m_current_step = Step::ReadBlocks;
        } else {
            wxString what = line.BeforeFirst(' ');
            wxString payload = line.AfterFirst(' ');

            _LOG("RTVARS line=["<<line<<"]");
            _LOG("RTVARS what=["<<what<<"]");
            _LOG("RTVARS payload=["<<payload<<"]");
            _LOG("RTVARS show_vars="<<config->show_vars<<" has_post_action="<<(m_action_post?1:0));

            if (config->show_vars || m_action_post) {
                if (what=="PROCESO" || what=="SUBPROCESO") {
                    if (m_vars_input_started) {
                        _LOG("RTVARS AddProc what=["<<what<<"] payload=["<<payload<<"] main="<<(what=="PROCESO")<<" immediate");
                        vars_window->AddProc(payload, what=="PROCESO");
                    } else {
                        _LOG("RTVARS buffer pending proc what=["<<what<<"] payload=["<<payload<<"] main="<<(what=="PROCESO"));
                        m_pending_proc_name = payload;
                        m_pending_proc_main = (what=="PROCESO");
                        m_pending_proc_valid = true;
                    }
                } else {
                    _LOG("RTVARS AddVar what=["<<what<<"] type=["<<(char)line.Last()<<"]");
                    std::fprintf(stderr, "DBG RTVARS AddVar what=[%s] last=%d\n",
             (const char*)_W2S(what), (int)line.Last());
std::fflush(stderr);
                    if (!m_vars_input_started) BeginVarsInputIfNeeded();
                    vars_window->AddVar(what, line.Last());
                }
            } else {
                if (what=="SUBPROCESO") {
                    _LOG("RTVARS RegisterAutocompKey sub=["<<line.AfterFirst(' ').BeforeFirst(':')<<"]");
                    vars_window->RegisterAutocompKey(line.AfterFirst(' ').BeforeFirst(':'));
                } else if (what!="PROCESO") {
                    _LOG("RTVARS RegisterAutocompKey var=["<<what.BeforeFirst('[')<<"]");
                    vars_window->RegisterAutocompKey(what.BeforeFirst('['));
                }
            }

            if (what=="PROCESO") {
                _LOG("RTVARS SetMainProcessTitleFromRTSM=["<<line.AfterFirst(' ').BeforeFirst(':')<<"]");
                src->SetMainProcessTitleFromRTSM(line.AfterFirst(' ').BeforeFirst(':'));
            }
        }
        break;

				case Step::ReadBlocks:
					_LOG("RTBLOCKS line=["<<line<<"]");
					if (line=="<!{[END_OF_BLOCKS]}!>") {
						std::fprintf(stderr, "DBG RTS COMPLETE stdout_bytes=%zu stderr_buf_bytes=%zu\n",
							m_stdout_bytes, m_stderr_buf.size());
						std::fflush(stderr);
						processing=false;
						_LOG("RTSyntaxManager::ContinueProcessing END_OF_BLOCKS src="<<src);
						src->RTOuputEnds();
						m_current_step = Step::PostAction;
						if (m_action_post) {
							_LOG("RTSyntaxManager::ContinueProcessing running post action");
							m_action_post();
							m_action_post = {};
						}
						m_current_step = Step::None;
						if (config->highlight_blocks) src->HighLightBlock();
						_LOG("RTSyntaxManager::ContinueProcessing EXIT complete src="<<src);
						RunPendingCycle("END_OF_BLOCKS");
						return;
					} else if (config->highlight_blocks) {
						long l1,l2;
						if (line.BeforeFirst(' ').ToLong(&l1) && line.AfterFirst(' ').ToLong(&l2)) {
							_LOG("RTBLOCKS AddBlock from="<<(l1-1)<<" to="<<(l2-1));
							src->AddBlock(l1-1,l2-1);
						}
					}
					break;

				case Step::PostAction:
					_LOG("RTSyntaxManager::ContinueProcessing ERROR Step::PostAction");
					_impossible;
					break;
			}
		} else {
			drain_stderr();
			++m_empty_reads;
			_LOG("RTSyntaxManager::ContinueProcessing EMPTY line, scheduling timer src="<<src<<" step="<<(int)m_current_step<<" processing="<<processing<<" empty_reads="<<m_empty_reads);
			std::fprintf(stderr, "DBG RTS EMPTY line step=%d empty_reads=%d processing=%d src=%p\n",
				(int)m_current_step,
				m_empty_reads,
				(int)processing,
				(void*)src);
			std::fflush(stderr);
			if ((m_current_step == Step::ReadErrors || m_current_step == Step::ReadVars || m_current_step == Step::ReadBlocks)
				&& m_empty_reads >= 50) {
				const char *phase = (m_current_step == Step::ReadErrors) ? "ReadErrors"
				                   : (m_current_step == Step::ReadVars) ? "ReadVars"
				                   : "ReadBlocks";
				_LOG("RTSyntaxManager::ContinueProcessing RT TIMEOUT in "<<phase<<" src="<<src<<" empty_reads="<<m_empty_reads);
				std::fprintf(stderr, "DBG RTS TIMEOUT %s empty_reads=%d src=%p processing=%d\n",
					phase,
					m_empty_reads,
					(void*)src,
					(int)processing);
				std::fprintf(stderr, "DBG RTS TIMEOUT stdout_bytes=%zu\n", m_stdout_bytes);
				std::fprintf(stderr, "DBG RTS TIMEOUT stderr_buf_bytes=%zu\n", m_stderr_buf.size());
				if (!m_stderr_buf.empty()) {
					std::fprintf(stderr, "DBG RTS TIMEOUT stderr_tail=[%s]\n", m_stderr_buf.c_str());
				}
				std::fflush(stderr);
				if (src) src->RTOuputEnds();
				TimeoutRecycle(phase, src);
				return;
			}
			timer->Start(100,true);
			return;
		}
	}
}

void RTSyntaxManager::OnTerminate (int pid, int status) {
	_LOG("RTSyntaxManager::OnTerminate pid="<<pid<<" status="<<status<<" src="<<src<<" restart="<<restart);
	running = false;
	if (restart) {
		Start(); 
		_LOG("RTSyntaxManager::OnTerminate RESTART DONE -> RUN PENDING has_pending="<<m_has_pending<<" pending_src="<<m_pending_src);
		std::fprintf(stderr, "DBG RTS RESTART DONE -> RUN PENDING has_pending=%d pending_src=%p\n",
			m_has_pending ? 1 : 0,
			(void*)m_pending_src);
		std::fflush(stderr);
		restart = false;
		RunPendingCycle("RESTART DONE");
	} else {
		the_one=NULL;
	}
}

bool RTSyntaxManager::IsLoaded ( ) {
	return the_one!=NULL;
}

RTSyntaxManager::~RTSyntaxManager ( ) {
	_LOG("RTSyntaxManager::~RTSyntaxManager this="<<this<<" pid="<<pid);
	timer->Stop();
	the_one=NULL;
}

void RTSyntaxManager::OnSourceClose(mxSource *_src) {
	_LOG("RTSyntaxManager::OnSourceClose _src="<<_src<<" the_one="<<the_one);
	if (!the_one) return;
	if (the_one->src==_src) {
		_LOG("RTSyntaxManager::OnSourceClose matched current src, clearing");
		the_one->src=NULL;
		the_one->ContinueProcessing();
	}
}
