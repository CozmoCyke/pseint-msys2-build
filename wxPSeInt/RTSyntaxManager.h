#ifndef RTSYNTAXMANAGER_H
#define RTSYNTAXMANAGER_H
#include <wx/process.h>
#include <functional>
#include <string>

class mxSource;
class wxTimer;

/**
* Esta clase gestiona el subproceso del int�rprete (pseint.bin) que se utiliza de 
* fondo para analizar la sintaxis (extrayendo tambien la lista de variables y los
* bloques para resaltar en amarillo). Hay una sola instancia, que es privada para
* que se maneja mediante los m�todos est�ticos, y estos m�todos controlan que 
* sea una y siempre que se requiera exista (similar a un singleton). La comunicacion
* con el proceso hijo va por tuberias, y el parseo de la salida lo hace la 
* funci�n ContinueProcessing. mxSource llama a Process(algo) cuando el c�digo 
* cambia y pasa cierto tiempo (controlado por su timer). Process setea todo en 
* cero, env�a el c�digo por la tuber�a y llama por primera vez a ContinueProcessing
* para que analice la respuesta. Si no hay respuesta rapida (el pseudoc�digo puede
* ser muy largo, la pc muy lenta, o lo que sea), usa un timer propio para relanzarse
* dentro de poco (el evento lo recibe la mainwindow y lo redirecciona mediante
* el m�todo Process(NULL), el mismo que antes pero con NULL como argumento).
* Esto es as� porque el wxYield (la facil ser�a un bucle con ContinueProcessing 
* y wxYield() adentro) puede traer problemas si justo se quiere ejecutar otra 
* cosa en medio (como el mism�simo algoritmo en la terminal).
**/

class RTSyntaxManager:public wxProcess {
	wxCSConv conv;
	wxTimer *timer;
	static RTSyntaxManager *the_one;
	int pid = -1;
	bool restart = false, running = false, processing = false;
	mxSource *src = nullptr;
	RTSyntaxManager();
	~RTSyntaxManager();
	void ContinueProcessing();
	void BeginVarsInputIfNeeded();
	void RunPendingCycle(const char *reason);
	void TimeoutRecycle(const char *phase, mxSource *src);
	void OnTerminate(int pid, int status);
	
	enum class Step { None, SendCode, ReadErrors, 
		              ReadVars, ReadBlocks, PostAction };
	Step m_current_step = Step::None;
	bool m_vars_input_started = false;
	bool m_pending_proc_valid = false;
	bool m_pending_proc_main = false;
	wxString m_pending_proc_name;
	int m_empty_reads = 0;
	std::string m_stderr_buf;
	size_t m_stdout_bytes = 0;
	mxSource* m_pending_src = nullptr;
	std::function<void()> m_pending_action_post;
	bool m_has_pending = false;
public:
	std::function<void()> m_action_post;
public:
	
	static void Start();
	static void Restart();
	static void Stop();
	static bool IsLoaded();
	static bool Process(mxSource *src = nullptr, std::function<void()> &&action_post = {});
	static void OnSourceClose(mxSource *src);
};

#endif

