#include "HelpManager.h"
#include <wx/textfile.h>
#include <wx/filename.h>
#include <iostream>
#include "ConfigManager.h"
#include "KeywordForms.h"
#include "../pseint/Keywords.hpp"
#include "string_conversions.h"

HelpManager *help = NULL;

static wxString GetAlgorithmHelpForm(KeywordType kw);

static void AddQuickHelpEntry(mxHashStringString &target, const wxString &key_raw, const wxString &file) {
	wxString key = key_raw;
	key.Trim(true); key.Trim(false);
	if (key.IsEmpty()) return;
	key.MakeLower();
	target[key] = file;
}

static void LoadQuickHelpEntries(const wxString &path, mxHashStringString &target) {
	wxTextFile fil(path);
	if (!fil.Open()) return;
	for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() ) {
		str.Trim(true); str.Trim(false);
		if (str.IsEmpty() || str[0]=='#' || str[0]==';')
			continue;
		int pos = str.Find('=');
		if (pos==wxNOT_FOUND)
			continue;
		wxString key = str.Left(pos);
		wxString val = str.Mid(pos+1);
		key.Trim(true); key.Trim(false);
		val.Trim(true); val.Trim(false);
		if (!key.IsEmpty() && !val.IsEmpty())
			AddQuickHelpEntry(target,key,val);
	}
	fil.Close();
}

static wxString NormalizeQuickHelpPage(wxString page) {
	page.Trim(true);
	page.Trim(false);
	if (page.IsEmpty())
		return page;
	wxFileName fn(page);
	if (fn.GetExt().IsEmpty())
		page << ".html";
	return page;
}

static wxString GetLegacyQuickHelp(wxString key, wxString def_val) {
	wxString str = def_val;
	key.MakeLower();
	const std::string key_std(_W2S(key));
	auto kw = [&](KeywordType type) { return IsKeywordAlternative(cfg_lang.keywords[type], key_std); };
	if (kw(KW_LEER)) str="lectura.html";
	else if (kw(KW_SUBALGORITMO) || kw(KW_FINSUBALGORITMO)) str="subprocesos.html";
	else if (kw(KW_ESCRIBIR)) str="escritura.html";
	else if (kw(KW_DIMENSIONAR) || kw(KW_REDIMENSIONAR)) str="arreglos.html";
	else if (kw(KW_DEFINIR) || kw(KW_ES) || kw(KW_COMO)) str="definir.html";
	else if (kw(KW_TIPO_ENTERO) || kw(KW_TIPO_REAL) || kw(KW_TIPO_LOGICO) || kw(KW_TIPO_CARACTER)) str="tipos.html";
	else if (kw(KW_SI) || kw(KW_FINSI) || kw(KW_ENTONCES) || kw(KW_SINO)) str="si.html";
	else if (kw(KW_REPETIR)) str="repetir.html";
	else if (kw(KW_PARA) || kw(KW_PARACADA) || kw(KW_DESDE) || kw(KW_HASTA) || kw(KW_CONPASO) || kw(KW_DE) || kw(KW_FINPARA)) str="para.html";
	else if (kw(KW_SEGUN) || kw(KW_OPCION) || kw(KW_DEOTROMODO) || kw(KW_FINSEGUN)) str="segun.html";
	else if (kw(KW_MIENTRAS) || kw(KW_FINMIENTRAS)) str="mientras.html";
	else if (kw(KW_ALGORITMO) || kw(KW_FINALGORITMO)) str="forma.html";
	else if (kw(KW_HACER)) str="hacer.html";
	else if (kw(KW_LIMPIARPANTALLA) || kw(KW_ESPERARTECLA) || kw(KW_ESPERARTIEMPO) || kw(KW_SEGUNDOS) || kw(KW_MILISEGUNDOS)) str="secotras.html";
	
	
	else if (key==GetAlgorithmHelpForm(KW_ALGORITMO).Lower()) str="forma.html";
	else if (key==GetAlgorithmHelpForm(KW_FINALGORITMO).Lower()) str="forma.html";
	
	
	else if (key==_Z("mayusculas")) str="funciones.html";
	else if (key==_Z("may?sculas")) str="funciones.html";
	else if (key==_Z("minusculas")) str="funciones.html";
	else if (key==_Z("min?sculas")) str="funciones.html";
	else if (key==_Z("longitud")) str="funciones.html";
	else if (key==_Z("subcadena")) str="funciones.html";
	else if (key==_Z("concatenar")) str="funciones.html";
	else if (key==_Z("convertiranumero")) str="funciones.html";
	else if (key==_Z("convertiran?mero")) str="funciones.html";
	else if (key==_Z("convertiratexto")) str="funciones.html";
	else if (key==_Z("subproceso")) str="subprocesos.html";
	else if (key==_Z("subprocesos")) str="subprocesos.html";
	else if (key==_Z("funci?n")) str="subprocesos.html";
	else if (key==_Z("funcion")) str="subprocesos.html";
	else if (key==_Z("subalgoritmo")) str="subprocesos.html";
	else if (key==_Z("finsubalgoritmo")) str="subprocesos.html";
	else if (key==_Z("sin")) str="escritura.html";
	else if (key==_Z("saltar")) str="escritura.html";
	else if (key==_Z("bajar")) str="escritura.html";
	else if (key==_Z("sinbajar")) str="escritura.html";
	else if (key==_Z("sinsaltar")) str="escritura.html";

	else if (key==_Z("dimension")) str="arreglos.html";
	else if (key==_Z("dimensi?n")) str="arreglos.html";
	else if (key==_Z("dimensionar")) str="arreglos.html";
	else if (key==_Z("redimension")) str="arreglos.html";
	else if (key==_Z("redimensi?n")) str="arreglos.html";
	else if (key==_Z("redimensionar")) str="arreglos.html";
	else if (key==_Z("matriz")) str="arreglos.html";
	else if (key==_Z("matrices")) str="arreglos.html";
	else if (key==_Z("arreglos")) str="arreglos.html";
	else if (key==_Z("arreglo")) str="arreglos.html";
	else if (key==_Z("vector")) str="arreglos.html";
	else if (key==_Z("vectores")) str="arreglos.html";
	else if (key==_Z("son")) str="definir.html";
	else if (key==_Z("definir")) str="definir.html";
	else if (key==_Z("definici?n")) str="definir.html";
	else if (key==_Z("definicion")) str="definir.html";
	else if (key==_Z("como")) str="definir.html";
	else if (key==_Z("num?rico")) str="tipos.html";
	else if (key==_Z("num?rica")) str="tipos.html";
	else if (key==_Z("numero")) str="tipos.html";
	else if (key==_Z("n?mero")) str="tipos.html";
	else if (key==_Z("logica")) str="tipos.html";
	else if (key==_Z("logico")) str="tipos.html";
	else if (key==_Z("cadena")) str="tipos.html";
	else if (key==_Z("texto")) str="tipos.html";
	else if (key==_Z("caracter")) str="tipos.html";
	else if (key==_Z("si")) str="si.html";
	else if (key==_Z("finsi")) str="si.html";
	else if (key==_Z("entonces")) str="si.html";
	else if (key==_Z("sino")) str="si.html";
	else if (key==_Z("asignar")) str="asignacion.html";
	else if (key==_Z("asignacion")) str="asignacion.html";
	else if (key==_Z("repetir")) str="repetir.html";
	else if (key==_Z("paso")) str="para.html";
	else if (key==_Z("finpara")) str="para.html";
	else if (key==_Z("para")) str="para.html";
	else if (key==_Z("cada")) str="para.html";
	else if (key==_Z("en")) str="para.html";
	else if (key==_Z("de")) str="para.html";
	else if (key==_Z("desde")) str="para.html";
	else if (key==_Z("hasta")) str="para.html";
	else if (key==_Z("opcion")) str="segun.html";
	else if (key==_Z("caso")) str="segun.html";
	else if (key==_Z("segun")) str="segun.html";
	else if (key==_Z("finsegun")) str="segun.html";
	else if (key==_Z("seg?n")) str="segun.html";
	else if (key==_Z("finseg?n")) str="segun.html";
	else if (key==_Z("finmientras")) str="mientras.html";
	else if (key==_Z("mientras")) str="mientras.html";
	else if (key==_Z("proceso")) str="forma.html";
	else if (key==_Z("finproceso")) str="forma.html";
	else if (key==_Z("algoritmo")) str="forma.html";
	else if (key==_Z("finalgoritmo")) str="forma.html";
	else if (key==_Z("operador")) str="operadores.html";
	else if (key==_Z("operadores")) str="operadores.html";
	else if (key==_Z("funciones")) str="funciones.html";
	else if (key==_Z("pi")) str="funciones.html";
	else if (key==_Z("euler")) str="funciones.html";
	else if (key==_Z("sen")) str="funciones.html";
	else if (key==_Z("asen")) str="funciones.html";
	else if (key==_Z("cos")) str="funciones.html";
	else if (key==_Z("acos")) str="funciones.html";
	else if (key==_Z("tan")) str="funciones.html";
	else if (key==_Z("atan")) str="funciones.html";
	else if (key==_Z("azar")) str="funciones.html";
	else if (key==_Z("aleatorio")) str="funciones.html";
	else if (key==_Z("trunc")) str="funciones.html";
	else if (key==_Z("rc")) str="funciones.html";
	else if (key==_Z("ln")) str="funciones.html";
	else if (key==_Z("redon")) str="funciones.html";
	else if (key==_Z("abs")) str="funciones.html";
	else if (key==_Z("fecha")) str="funciones.html";
	else if (key==_Z("hora")) str="funciones.html";
	else if (key==_Z("fechaactual")) str="funciones.html";
	else if (key==_Z("horaactual")) str="funciones.html";
	else if (key==_Z("hacer")) str="hacer.html";
	else if (key==_Z("tipos")) str="tipos.html";
	else if (key==_Z("tipo")) str="tipos.html";
	else if (key==_Z("otro")) str="segun.html";
	else if (key==_Z("modo")) str="segun.html";
	else if (key==_Z("sies")) str="segun.html";
	else if (key==_Z("case")) str="segun.html";
	else if (key==_Z("mod")) str="operadores.html";
	else if (key==_Z("divisible")) str="colcond.html";
	else if (key==_Z("multiplo")) str="colcond.html";
	else if (key==_Z("positivo")) str="colcond.html";
	else if (key==_Z("negativo")) str="colcond.html";
	else if (key==_Z("impar")) str="colcond.html";
	else if (key==_Z("par")) str="colcond.html";
	else if (key==_Z("cero")) str="colcond.html";
	else if (key==_Z("distinto")) str="colcond.html";
	else if (key==_Z("igual")) str="colcond.html";
	else if (key==_Z("es")) str="colcond.html";
	else if (key==_Z("menor")) str="colcond.html";
	else if (key==_Z("mayor")) str="colcond.html";
	else if (key==_Z("esperartecla")) str="secotras.html";
	else if (key==_Z("tecla")) str="secotras.html";
	else if (key==_Z("esperar")) str="secotras.html";
	else if (key==_Z("limpiar")) str="secotras.html";
	else if (key==_Z("limpiarpantalla")) str="secotras.html";
	else if (key==_Z("borrar")) str="secotras.html";
	else if (key==_Z("borrarpantalla")) str="secotras.html";
	else if (key==_Z("pantalla")) str="secotras.html";
	else if (key==_Z("variable")) str="variables.html";
	else if (key==_Z("variables")) str="variables.html";
	else if (key==_Z("ejemplo")) str="ejemplos.html";
	else if (key==_Z("ejemplos")) str="ejemplos.html";
	return str;
}

static wxString GetAlgorithmHelpForm(KeywordType kw) {
	const std::string &lang = LocalizationManager::Instance().GetCurrentLanguage();
	if (kw==KW_ALGORITMO) {
		if (lang=="fr") return "Algorithme";
		if (lang=="en") return "Algorithm";
		return "Proceso";
	}
	if (kw==KW_FINALGORITMO) {
		if (lang=="fr") return "FinAlgorithme";
		if (lang=="en") return "EndAlgorithm";
		return "FinProceso";
	}
	return wxString();
}

HelpManager::HelpManager() {
	LoadErrors();
	LoadCommands();
	LoadQuickHelp();
}

void HelpManager::LoadCommands() {
	std::cerr << "HelpManager::LoadCommands path=" << _W2S(config->GetHelpFilePath("comandos.hlp")) << std::endl;
	wxTextFile fil(config->GetHelpFilePath("comandos.hlp"));
	if (!fil.Exists()) return;
	fil.Open();
	wxString command, help_text;
	for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() ) {
		if (str[0]==' ') {
			help_text<<str;//<<"<br>";
		} else {
			commands[command] = help_text;
			help_text.Clear();
			command=str;
		}
	}
	commands[command] = help_text;
	fil.Close();
}

void HelpManager::LoadErrors() {
	for(int k=0;k<2;++k) {
		wxString name = k==0 ? "errores.hlp" : "warnings.hlp";
		std::cerr << "HelpManager::LoadErrors path=" << _W2S(config->GetHelpFilePath(name)) << std::endl;
		wxTextFile fil(config->GetHelpFilePath(name));
		if (not fil.Open()) continue;
		wxString command("0"), help_text;
		for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() ) {
			if (str[0]==' ') {
				help_text << str << "<br>";
			} else {
				long e = 0; command.ToLong(&e);
				errors[e] = help_text;
				help_text.Clear();
				command = str;
			}
		}
		long e=0; command.ToLong(&e);
		errors[e] = help_text;
		fil.Close();
	}
}

void HelpManager::LoadQuickHelp() {
	quickhelp.clear();
	std::cerr << "HelpManager::LoadQuickHelp path=" << _W2S(config->GetHelpFilePath("quickhelp.map")) << std::endl;
	LoadQuickHelpEntries(config->GetHelpFilePath("quickhelp.map"), quickhelp);
}

wxString HelpManager::GetErrorText(const wxString &text, int num) {
	wxString ret(_Z("<HTML><HEAD><TITLE>PSeInt QuickHelp</TITLE></HEAD><BODY><B>"));
	if (num>=MAX_ERRORS_TXT || errors[num].Len()==0) 
		ret<<utils->ToHtml(text)<<_Z("</B><BR><BR> No se encontro descripci?n para este error <BR><BR></BODY></HTML>");
	else
		ret<<utils->ToHtml(text)<<_Z("</B><BR><BR>")<<errors[num]<<_Z("<BR><BR></BODY></HTML>");
	return ret;
}

wxString HelpManager::GetCommandText(const wxString &com) {
	wxString ret(_Z("<HTML><HEAD><TITLE>PSeInt QuickHelp</TITLE></HEAD><BODY><B>"));
	ret<<com<<_Z("</B><BR><BR>")<<commands[com]<<_Z("<BR><BR></BODY></HTML>");
	return ret;
}

wxString HelpManager::GetQuickHelp(wxString key, wxString def_val) {
	wxString str = def_val;
	key.MakeLower();
	wxString runtime_help = GetKeywordHelpPageForWord(key);
	wxString runtime_category = GetKeywordCategoryForWord(key);
	wxString runtime_canonical = GetKeywordCanonicalForWord(key);
	std::cerr << "HelpManager::GetQuickHelp key=" << _W2S(key) << " def=" << _W2S(def_val) << " runtime_help=" << _W2S(runtime_help) << std::endl;
	if (!runtime_help.IsEmpty())
		str = runtime_help;
	else {
		if (!runtime_canonical.IsEmpty() && (runtime_category == "predef" || runtime_category == "constant")) {
			std::cerr
				<< "[KW][META][" << (runtime_category == "constant" ? "CONSTANT" : "PREDEF") << "] "
				<< "canonical=" << _W2S(runtime_canonical)
				<< " field=help"
				<< " source=fallback"
				<< " lang=" << LocalizationManager::Instance().GetCurrentLanguage()
				<< std::endl;
		}
		mxHashStringString::iterator it = quickhelp.find(key);
		if (it!=quickhelp.end())
			str = it->second;
		else
			str = GetLegacyQuickHelp(key, def_val);
	}
	str = NormalizeQuickHelpPage(str);
	if (def_val.Len()) {
		wxString resolved = config->GetHelpFilePath(str);
		std::cerr << "HelpManager::GetQuickHelp final=" << _W2S(resolved) << std::endl;
		str = resolved;
	}
	return str;
}
