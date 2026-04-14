#include <cstring>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include "version.h"
#include "zcurlib.h"
#include "common.h"
#include "intercambio.h"
#include "new_memoria.h"
#include "global.h"
#include "utils.h"
#include "Evaluar.hpp"
#include "SynCheck.hpp"
#include "Ejecutar.hpp"
#include "FuncsManager.hpp"
#include "Programa.hpp"
#include "ErrorHandler.hpp"
#include "case_map.h"
#include "RunTime.hpp"
#include "ProgramaDump.hpp"
#include "LocalizationManager.h"
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

static bool g_encoding_test = false;

static void InitConsoleEncoding() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
}

static void EncodingProbe() {
	if (!g_encoding_test && std::getenv("PSEINT_ENCODING_TEST") == nullptr) return;
	std::fprintf(stderr, "ENCODING TEST fprintf: Definir Reel Caractere\\n");
	std::cout << "ENCODING TEST cout: Definir Reel Caractere" << std::endl;
}

void on_signal(int s) {
	Inter.SetFinished(true);
	Inter.UnInit();
	exit(s);
}

void checksum(Programa &prog) {
	if (prog.GetInstCount()==3) {
		string &s=prog[1].instruccion;
		int n=0,p=1;
		for(unsigned int i=0;i<s.size();i++) { 
			n+=s[i]; p=(p*s[i])%1000000;
		}
		if (n==839 && p==102912) { // daba 730880, por que cambio???
			prog[1].instruccion[9]-=11;
			prog[1].instruccion[10]+=10;
			prog[1].instruccion[11]-=7;
			prog[1].instruccion.insert(12,",\"!\"");
		}
	} else if (prog.GetInstCount()==4) {
		string &s1=prog[1].instruccion;
		string &s2=prog[2].instruccion;
		int n1=0,n2=0,p1=1,p2=1;
		for(unsigned int i=0;i<s1.size();i++) { 
			n1+=s1[i]; p1=(p1*s1[i])%1000000;
		}
		for(unsigned int i=0;i<s2.size();i++) { 
			n2+=s2[i]; p2=(p2*s2[i])%1000000;
		}
		if (n1==619 && p1==956160 && n2==921 && p2==673152) {
			string s = s2+s2+s2; 
			int x[]={-59,-51,-35,-50,-41,-34,-41,-50,63,-63,-46,-52,
				     -27,-23,12,28,-42,-34,-25,-13,-50,35,44,19,15,48,
				     -36,-50,-34,-72,-41,-34,-41,-50,60,22,17,11,-18};
			for(size_t i=0;i<s.size();i++) s[i]+=x[i]; 
			predef_input.push(s);
		}
	}
}

LangSettings lang(LS_DO_NOT_INIT);

//----------------------------------------------------------------------------
// ************************* Programa Principal ******************************
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	InitConsoleEncoding();
    std::cerr << "LANGTRACE before_set_exepath" << std::endl; 
	SetExecutablePath(argv[0]);
	std::cerr << "LANGTRACE after_set_exepath" << std::endl;
	signal(2,on_signal);
	signal(6,on_signal);
	signal(9,on_signal);
	signal(15,on_signal);

	_handle_version_query("PSeInt",0);
	
	lang.Reset();
	
	RunTime rt;

	bool 
		check=true, // checkear syntaxis?
		run=true, // ejecutar?
		user=true, // 
		log_file=false, // guardar errores en un archivo de log
		error=false, 
		for_draw=false, // generar entrada para psdraw
		for_export=false, // generar entrada para psexport
		real_time_syntax=false; // indica que espera eternamente codigo desde std, para usar de fondo para el checkeo de sintaxis en tiempo real en la gui
	int 
		forced_seed=-1; // semilla a utilizar para inicializar la generaci?n de n?meros aleatorios (si es -1 se usa el reloj)
	
	int fil_count=0,delay=0; char *fil_args[5];
	for (int i=1;i<argc;i++) {
		if (argv[i][0]=='-') {
			string str(argv[i]);
			if (str.substr(0,7)=="--seed=")
				forced_seed=atoi(str.substr(7).c_str());
			else if (str.substr(0,8)=="--delay=")
				delay=atoi(str.substr(8).c_str());
			else if (str.substr(0,8)=="--input=")
				predef_input.push(str.substr(8));
			else if (str.substr(0,7)=="--port=")
				Inter.SetPort(atoi(str.substr(7).c_str()));
			else if (str=="--rawerrors")
				raw_errors=true;
			else if (str=="--fortest")
				for_test=true;
			else if (str=="--preservecomments")
				preserve_comments=true;
			else if (str=="--norun")
				run=false;
			else if (str=="--color")
				colored_output=true;
			else if (str=="--nocheck")
				check=false;
			else if (str=="--nouser")
				user=false;
		else if (str=="--noinput")
			noinput=true;
		else if (str=="--encodingtest")
			g_encoding_test=true;
		else if (str=="--fixwincharset")
			fix_win_charset=true;
			else if (str=="--draw") {
				InitCaseMap();
				ignore_logic_errors = for_draw = true;
				run=false;
			} else if (str=="--export") {
				for_export = for_draw = true;
				run=false;
			} else if (str=="--easteregg") {
				cerr<<"Bazinga!"<<endl;
				return 0;
			} else if (str=="--forrealtimesyntax") {
				InitCaseMap();
				real_time_syntax=true;
			} else if (str=="--forpseintterminal") {
				for_pseint_terminal=true;
			} else if (str=="--withioreferences") {
				with_io_references=true;
			} else if (str=="--foreval") {
				for_eval=true;
			} else if (str.substr(0,10)=="--profile=") {
				if (not lang.Load(str.substr(10))) {
					cerr << LocalizationManager::Instance().Translate("No se pudo leer el archivo de perfil: ") << str.substr(10) << endl;
					exit(1);				}
			} else if (str.substr(0,7)=="--lang=") {
				std::string lang_code = str.substr(7);
				std::string profile_before_lang = lang.GetAsSingleString();
				std::cerr << "LANGTRACE step=parse arg=" << lang_code << std::endl;
				std::cerr << "LANGTRACE preserved_profile=" << profile_before_lang << std::endl;
				
				std::cerr << "LANGKW ALGORITMO=" << lang.keywords[KW_ALGORITMO].get() << std::endl;
std::cerr << "LANGKW FINALGORITMO=" << lang.keywords[KW_FINALGORITMO].get() << std::endl;
std::cerr << "LANGKW DEFINIR=" << lang.keywords[KW_DEFINIR].get() << std::endl;
std::cerr << "LANGKW COMO=" << lang.keywords[KW_COMO].get() << std::endl;
				
				std::string current_lang_before_set = LocalizationManager::Instance().GetCurrentLanguage();
				std::cerr << "LANGTRACE before_setlanguage_call lang_code=" << lang_code << std::endl;
				LocalizationManager::Instance().SetLanguage(lang_code);
				std::cerr << "LANGTRACE after_setlanguage_call" << std::endl;
				std::cerr << "LANGTRACE postset ALGORITMO=" << lang.keywords[KW_ALGORITMO].get() << std::endl;
    std::cerr << "LANGTRACE postset FINALGORITMO=" << lang.keywords[KW_FINALGORITMO].get() << std::endl;
    std::cerr << "LANGTRACE postset DEFINIR=" << lang.keywords[KW_DEFINIR].get() << std::endl;
    std::cerr << "LANGTRACE postset COMO=" << lang.keywords[KW_COMO].get() << std::endl;
				std::string current_lang_after_set = LocalizationManager::Instance().GetCurrentLanguage();
				std::cerr << "LANGTRACE set_current_lang before=" << current_lang_before_set << " after=" << current_lang_after_set << " source=main" << std::endl;
				
				// Reset keyword tables for the new language, then restore the already selected profile flags.
				lang.Reset();
				lang.SetFromSingleString(profile_before_lang);
				lang.Fix();
				std::cerr << "LANGTRACE restored_profile=" << lang.GetAsSingleString() << std::endl;
				std::cerr << "LANGTRACE restored_overload_equal=" << (lang[LS_OVERLOAD_EQUAL] ? 1 : 0) << std::endl;
				
				std::cerr << "LANGTRACE engine current_lang=" << LocalizationManager::Instance().GetCurrentLanguage() << std::endl;
			} else if (str.substr(0,2)=="--" && !lang.ProcessConfigLine(str.substr(2))) {
				error=true;
			}
		} else {
			fil_args[fil_count++]=argv[i];
		}
	}
	EncodingProbe();
	lang.Fix();
	std::cerr << "LANGTRACE postfix current_lang=" << LocalizationManager::Instance().GetCurrentLanguage() << std::endl;
	std::cerr << "LANGTRACE postfix overload_equal=" << (lang[LS_OVERLOAD_EQUAL] ? 1 : 0) << std::endl;
	std::cerr << "LANGTRACE postfix ALGORITMO=" << lang.keywords[KW_ALGORITMO].get() << std::endl;
	std::cerr << "LANGTRACE postfix FINALGORITMO=" << lang.keywords[KW_FINALGORITMO].get() << std::endl;
	std::cerr << "LANGTRACE postfix DEFINIR=" << lang.keywords[KW_DEFINIR].get() << std::endl;
	std::cerr << "LANGTRACE postfix COMO=" << lang.keywords[KW_COMO].get() << std::endl;
	std::cerr << "LANGTRACE postfix LEER=" << lang.keywords[KW_LEER].get() << std::endl;
	std::cerr << "LANGTRACE postfix ESCRIBIR=" << lang.keywords[KW_ESCRIBIR].get() << std::endl;
	error = error&&((fil_count<(for_draw?2:1))||(fil_count>(for_draw?3:2)));
	log_file = fil_count>(for_draw?2:1);
#ifdef __APPLE__
	wait_key=false;
#else
	wait_key=user&&!for_pseint_terminal;
#endif

	// comprobar parametros
	if (error || (fil_count==0 && !real_time_syntax)) {
		cout<<"Use: pseint FileName.psc [<options>] [LogFile]\n";
		cout<<"     pseint FileName.psc --draw DrawTempFile.psd [LogFile]\n";
		cout<<LocalizationManager::Instance().Translate(" <options> puede ser una o mas de las siguientes:")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --color                utilizar colores para formatear la salida")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --nocheck              no revisar la sintaxis")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --norun                no ejecutar")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --nouser               no mostrar mensajes de estado ni esperar un tecla al final")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --port=<num>           define el puerto tpc para comunicar controlar la depuracion")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --fortest              ignora algunas instrucciones particulares para evitar ciertas entradas/salidas")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --rawerrors            muestra los errores sin descripcion, para testing automatizado")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --noinput              en lugar realizar las lecturas desde el teclado, lo hace desde los argumentos")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --input=<str>          sirve para predefinir uno o m?s valores de entrada para acciones LEER")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --fixwincharset        corrige la codificaci?n de algunos caracteres para que se muestren correctamente")<<endl;
		cout<<LocalizationManager::Instance().Translate("                             en la consola de Windows")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --writepositions       al generar el archivo parseado para el editor de diagrams de flujo incluye los")<<endl;
		cout<<"                             numeros de linea e instruccion necesarios para marcar la ejecucion paso a paso"<<endl;
		cout<<LocalizationManager::Instance().Translate("      --delay=<num>          define el retardo entre instrucciones para la ejecucion paso a paso")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --seed=<num>           semilla para el generador de numeros aleatorios")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --profile=<archivo>    archivo de perfil a utilizar para configurar el int?rprete")<<endl;
		cout<<LocalizationManager::Instance().Translate("      --<opt>=?              cambia la opcion <opt> del perfil, ? puede ser 0 o 1")<<endl;
		exit(1);
	}
	
	if (for_eval) { colored_output=false; user=wait_key=false; }
	
	if (colored_output) { 
#ifndef __APPLE__
		setBackColor(COLOR_BLACK); 
#endif
		clrscr();
	}
	
	if (log_file) {
		ExeInfo.open(fil_args[for_draw?2:1]);
		if (ExeInfo.is_open()) ExeInfoOn=true;
	}

	// inicializaciones varias
	rt.funcs.LoadPredefs();
	if (forced_seed==-1) srand(time(NULL)); else srand(forced_seed);
	
	Programa &programa = rt.prog;
	ErrorHandler &err_handler = rt.err;
	
	if (real_time_syntax) {
		int rt_cycle = 0;
		while (cin) {
			std::fprintf(stderr, "DBG PSEINT RT BEGIN CYCLE cycle=%d\n", rt_cycle);
			std::fflush(stderr);
//			memoria->HardReset();
			programa.HardReset();
			rt.funcs.UnloadSubprocesos();
			err_handler.Reset();
			string line;
			int lcount=0;
			bool got_input=false;
			while (cin) {
				getline(cin,line);
				if (!cin) break;
				lcount++;
				got_input=true;
				std::fprintf(stderr, "DBG PSEINT RT READ line=[%s] cycle=%d lcount=%d\n", line.c_str(), rt_cycle, lcount);
				std::fflush(stderr);
				if (line=="<!{[EXIT]}!>") { rt.funcs.UnloadPredefs(); return 0; }
				if (line=="<!{[END_OF_INPUT]}!>") {
					std::fprintf(stderr, "DBG PSEINT RT END_OF_INPUT detected cycle=%d\n", rt_cycle);
					std::fflush(stderr);
					break;
				}
				programa.PushBack(line);
			}
			if (!got_input) {
				std::fprintf(stderr, "DBG PSEINT RT EMPTY CYCLE cycle=%d\n", rt_cycle);
				std::fflush(stderr);
				break;
			}

			std::fprintf(stderr, "DBG PSEINT RT before SynCheck cycle=%d prog_count=%d\n", rt_cycle, programa.GetInstCount());
			std::fflush(stderr);
			SynCheck(rt);
			std::fprintf(stderr, "DBG PSEINT RT after SynCheck cycle=%d ok=%d errors=%d main=%d subs=%zu\n",
				rt_cycle,
				err_handler.IsOk() ? 1 : 0,
				err_handler.GetErrorsCount(),
				rt.funcs.HaveMain() ? 1 : 0,
				rt.funcs.GetAllSubs().size());
			std::fflush(stderr);
			std::fprintf(stderr, "DBG PSEINT RT emit END_OF_OUTPUT cycle=%d\n", rt_cycle);
			std::fflush(stderr);
			cout<<"<!{[END_OF_OUTPUT]}!>"<<endl;
			std::fprintf(stderr, "DBG PSEINT RT begin vars emit cycle=%d subs=%zu\n", rt_cycle, rt.funcs.GetAllSubs().size());
			std::fflush(stderr);
			for(auto &pf : rt.funcs.GetAllSubs()) {
				auto &func = pf.second;
				const std::string &name = pf.first;
				for(int j=0;j<=func->GetArgsCount();j++) { // agregar argumentos y valor de retorno de la funcion
					if (func->nombres[j].size() && !func->memoria->Existe(func->nombres[j]))
						func->memoria->DefinirTipo(func->nombres[j],vt_desconocido);
				}
				if (name!=rt.funcs.GetMainName()) cout<<"SUB";
				cout<<"PROCESO ";
				if (case_map) cout<<(*case_map)[name];
				else cout<<name;
				cout<<":"<<func->userline_start-1<<':'<<func->userline_end-1<<endl;
				func->memoria->ListVars(case_map);
			}
			std::fprintf(stderr, "DBG PSEINT RT emit END_OF_VARS cycle=%d\n", rt_cycle);
			std::fflush(stderr);
			cout<<"<!{[END_OF_VARS]}!>"<<endl;
			std::fprintf(stderr, "DBG PSEINT RT after END_OF_VARS cycle=%d\n", rt_cycle);
			std::fflush(stderr);
			if (lcount) {
				int n=programa.GetInstCount();
				int *bk=new int[lcount], *st=new int[n+1], stn=0;
				for(int i=0;i<lcount;i++) bk[i]=-1;
				for(int i=0;i<n;i++) { 
					Instruccion &in=programa[i];
					if (in==IT_SI || in==IT_PARA || in==IT_PARACADA ||
						in==IT_MIENTRAS || in==IT_SEGUN || in==IT_REPETIR)
							st[stn++]=in.loc.linea;
					else if (stn&& (in==IT_FINSI || in==IT_FINMIENTRAS || 
									in==IT_FINSEGUN || in==IT_FINPARA ||
									in==IT_HASTAQUE))
							bk[st[--stn]]=in.loc.linea;
				}
				for(int i=0;i<lcount;i++) { 
					if (bk[i]!=-1) cout<<i<<" "<<bk[i]<<endl;
				}
				delete []bk; delete []st;
			}
			std::fprintf(stderr, "DBG PSEINT RT emit END_OF_BLOCKS cycle=%d\n", rt_cycle);
			std::fflush(stderr);
			cout<<"<!{[END_OF_BLOCKS]}!>"<<endl;
			std::fprintf(stderr, "DBG PSEINT RT after END_OF_BLOCKS cycle=%d\n", rt_cycle);
			std::fflush(stderr);
			++rt_cycle;
		}
		return 0; // nunca se llega hasta aqui
	}
	
	// Leer el archivo	
	char *filename=fil_args[0];
	ifstream archivo(filename);
	if (!archivo.is_open()) {
		if (colored_output) setForeColor(COLOR_ERROR);
		cout<<LocalizationManager::Instance().Translate("ERROR: No se pudo abrir el archivo \\\"")<<filename<<"\""<<endl;
		if (ExeInfoOn) {
			ExeInfo<<LocalizationManager::Instance().Translate("ERROR: No se pudo abrir el archivo \\\"")<<filename<<"\""<<endl;
			if (user) {
				hideCursor();
				if (wait_key) getKey();
				showCursor();
			}
		}
		exit(1);
	}
	
	if (user && check) show_user_info(LocalizationManager::Instance().Translate("Leyendo archivo y comprobando sintaxis..."));
	for(string buffer;getline(archivo,buffer);)
		programa.PushBack(buffer);
	archivo.close();

	// comprobar sintaxis y ejecutar
	if (with_io_references) // asumo que esto equivale a que el cliente es psterm
		err_handler.DisableCompileTimeWarnings();
	SynCheck(rt);
	Inter.InitDebug(rt,delay);
	
	if (for_pseint_terminal) { cout<<"\033[zt"<<rt.funcs.GetMainName()<<"\n"; }

	//ejecutar
	if (err_handler.IsOk()) {
	// salida para diagrama (temporal, hasta que psdraw y psexport consuman Programa directamente)
		if (for_draw) {
			if (case_map) {
//				CaseMapPurge();
				CaseMapApply(rt,programa,for_export);
			}
			SavePrograma(fil_args[1],programa);
			if (user) show_user_info(LocalizationManager::Instance().Translate("Dibujo guardado."));
			ExeInfo.close();
			ExeInfoOn=false;
		} else if (run) {
			for(auto &pf : rt.funcs.GetAllSubs()) pf.second->memoria->FakeReset();
			checksum(programa);
			Inter.SetStarted();
			const Funcion *main_func = rt.funcs.GetMainFunc();
			memoria = main_func->memoria.get();
			Inter.SetLocation(rt.prog[main_func->line_start].loc);
			if (ExeInfoOn) if (user) ExeInfo<<LocalizationManager::Instance().Translate("*** Ejecucion Iniciada. ***")<<endl;
			if (user) show_user_info(LocalizationManager::Instance().Translate("*** Ejecucion Iniciada. ***"));
			Ejecutar(rt,main_func->line_start);
			Inter.SetFinished();
			if (ExeInfoOn) ExeInfo<<LocalizationManager::Instance().Translate("*** Ejecucion Finalizada. ***");
			if (user) show_user_info(LocalizationManager::Instance().Translate("*** Ejecucion Finalizada. ***"));
		}
	} else {
		if (user) cout<<endl;
		int errores = rt.err.GetErrorsCount();
		if (errores==1) {
			if (ExeInfoOn) if (user) ExeInfo<<LocalizationManager::Instance().Translate("*** Se encontro 1 error. ***");
			if (user) show_user_info(LocalizationManager::Instance().Translate("*** Se encontro 1 error. ***"));
		} else {
			if (ExeInfoOn) if (user) ExeInfo<<LocalizationManager::Instance().Translate("*** Se encontraron ")<< " " <<errores<< " " << LocalizationManager::Instance().Translate(" errores. ***");
			if (user) show_user_info(LocalizationManager::Instance().Translate("*** Se encontraron ")+" ",errores," " +LocalizationManager::Instance().Translate(" errores. ***"));
		}
	}
	// esperar un enter si es necesario
	if (ExeInfoOn) {   
		ExeInfo.close();
	}
	if (user && !for_draw) {
		hideCursor();
		if (wait_key) getKey();
		showCursor();
	}

	return 0;
}


//--------------------------------------------------------------------------------------
