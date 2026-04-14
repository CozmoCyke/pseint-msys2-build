#include <cmath>
#include <ctime>
#include "FuncsManager.hpp"
#include "ErrorHandler.hpp"
#include "LangSettings.h"
#include "global.h"
#include "LocalizationManager.h"

DataValue func_rc(ErrorHandler &err_handler, DataValue *arg) {
	double x = arg[0].GetAsReal();
	if (x < 0) {
		err_handler.ErrorIfRunning(147, LocalizationManager::Instance().Translate("Raiz cuadrada de numero negativo."));
		return DataValue::MakeEmpty(vt_numerica);
	}
	return DataValue::MakeReal(std::sqrt(x));
}

DataValue func_abs(ErrorHandler &err_handler, DataValue *arg) {
	double d = arg[0].GetAsReal();
	return DataValue::MakeReal(d < 0 ? -d : d);
}

DataValue func_ln(ErrorHandler &err_handler, DataValue *arg) {
	double x = arg[0].GetAsReal();
	if (x <= 0) {
		err_handler.ErrorIfRunning(148, LocalizationManager::Instance().Translate("Logaritmo de 0 o negativo."));
		return DataValue::MakeEmpty(vt_numerica);
	}
	return DataValue::MakeReal(log(x));
}

DataValue func_exp(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(exp(arg[0].GetAsReal()));
}

DataValue func_sen(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(sin(arg[0].GetAsReal()));
}

DataValue func_asen(ErrorHandler &err_handler, DataValue *arg) {
	double x = arg[0].GetAsReal();
	if (x < -1 || x > +1) {
		err_handler.ErrorIfRunning(312, LocalizationManager::Instance().Translate("Argumento invalido para la funcion ASEN (debe estar en [-1;+1])."));
		return DataValue::MakeEmpty(vt_numerica);
	}
	return DataValue::MakeReal(asin(x));
}

DataValue func_acos(ErrorHandler &err_handler, DataValue *arg) {
	double x = arg[0].GetAsReal();
	if (x < -1 || x > +1) {
		err_handler.ErrorIfRunning(312, LocalizationManager::Instance().Translate("Argumento invalido para la funcion ACOS (debe estar en [-1;+1])."));
		return DataValue::MakeEmpty(vt_numerica);
	}
	return DataValue::MakeReal(acos(x));
}

DataValue func_cos(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(cos(arg[0].GetAsReal()));
}

DataValue func_tan(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(tan(arg[0].GetAsReal()));
}

DataValue func_atan(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(atan(arg[0].GetAsReal()));
}

DataValue func_azar(ErrorHandler &err_handler, DataValue *arg) {
	int x = arg[0].GetAsInt();
	if (x <= 0) {
		err_handler.ErrorIfRunning(306, LocalizationManager::Instance().Translate("Azar de 0 o negativo."));
		return DataValue::MakeEmpty(vt_numerica_entera);
	} else {
		return DataValue::MakeInt(rand() % x);
	}
}

DataValue func_fecha(ErrorHandler &err_handler, DataValue *arg) {
	std::time_t now = std::time(nullptr);
	std::tm *gt = std::gmtime(&now);
	int aa = gt->tm_year + 1900, mm = gt->tm_mon + 1, dd = gt->tm_mday;
	return DataValue::MakeInt(aa * 10000 + mm * 100 + dd);
}

DataValue func_hora(ErrorHandler &err_handler, DataValue *arg) {
	std::time_t now = std::time(nullptr);
	std::tm *gt = std::localtime(&now);
	int hh = gt->tm_hour, mm = gt->tm_min, ss = gt->tm_sec;
	return DataValue::MakeInt(hh * 10000 + mm * 100 + ss);
}

DataValue func_aleatorio(ErrorHandler &err_handler, DataValue *arg) {
	int a = arg[0].GetAsInt();
	int b = arg[1].GetAsInt();
	if (b < a) {
		int x = a;
		a = b;
		b = x;
	}
	return DataValue::MakeInt(a + rand() % (b - a + 1));
}

DataValue func_trunc(ErrorHandler &err_handler, DataValue *arg) {
	double dbl = arg[0].GetAsReal();
	int i = int(dbl);
	// intentar compensar algunos errores numéricos... que al menos parezca el
	// número que se ve al escribir, el cual se redondea a 10 decimales...
	// por eso uso un epsilon un poquito más chico que eso
	if ((i + 1) - dbl < 1e-11) i++;
	else if ((i - 1) - dbl > -1e-11) i--;
	return DataValue::MakeInt(i);
}

DataValue func_redon(ErrorHandler &err_handler, DataValue *arg) {
	double x = arg[0].GetAsReal();
	return DataValue::MakeInt(int(x + (x < 0 ? -.5 : +.5)));
}

DataValue func_longitud(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeInt(arg[0].GetAsString().size());
}

DataValue func_mayusculas(ErrorHandler &err_handler, DataValue *arg) {
	std::string s = arg[0].GetAsString();
	size_t l = s.size();
	for (size_t i = 0; i < l; i++) {
		unsigned char c = static_cast<unsigned char>(s[i]);
		if (s[i] >= 'a' && s[i] <= 'z') s[i] += 'A' - 'a';
		else if (c == 0xE1) s[i] = '\xC1'; // á -> Á
		else if (c == 0xE9) s[i] = '\xC9'; // é -> É
		else if (c == 0xED) s[i] = '\xCD'; // í -> Í
		else if (c == 0xF3) s[i] = '\xD3'; // ó -> Ó
		else if (c == 0xFA) s[i] = '\xDA'; // ú -> Ú
		else if (c == 0xFC) s[i] = '\xDC'; // ü -> Ü
		else if (c == 0xF1) s[i] = '\xD1'; // ñ -> Ñ
	}
	return DataValue::MakeString(s);
}

DataValue func_minusculas(ErrorHandler &err_handler, DataValue *arg) {
	std::string s = arg[0].GetAsString();
	size_t l = s.length();
	for (size_t i = 0; i < l; i++) {
		unsigned char c = static_cast<unsigned char>(s[i]);
		if (s[i] >= 'A' && s[i] <= 'Z') s[i] += 'a' - 'A';
		else if (c == 0xC1) s[i] = '\xE1'; // Á -> á
		else if (c == 0xC9) s[i] = '\xE9'; // É -> é
		else if (c == 0xCD) s[i] = '\xED'; // Í -> í
		else if (c == 0xD3) s[i] = '\xF3'; // Ó -> ó
		else if (c == 0xDA) s[i] = '\xFA'; // Ú -> ú
		else if (c == 0xDC) s[i] = '\xFC'; // Ü -> ü
		else if (c == 0xD1) s[i] = '\xF1'; // Ñ -> ñ
	}
	return DataValue::MakeString(s);
}

DataValue func_subcadena(ErrorHandler &err_handler, DataValue *arg) {
	std::string s = arg[0].GetAsString();
	int l = s.length(), f = arg[1].GetAsInt(), t = arg[2].GetAsInt();
	if (!lang[LS_BASE_ZERO_ARRAYS]) {
		f--;
		t--;
	}
	if (t > l - 1) t = l - 1;
	if (f < 0) f = 0;
	if (t < f) return DataValue::MakeEmpty(vt_caracter);
	return DataValue::MakeString(s.substr(f, t - f + 1));
}

DataValue func_concatenar(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeString(arg[0].GetAsString() + arg[1].GetAsString());
}

DataValue func_atof(ErrorHandler &err_handler, DataValue *arg) {
	std::string s = arg[0].GetAsString();
	bool punto = false;
	int j = 0;
	if (s.size() && (s[0] == '+' || s[0] == '-')) j++;
	for (unsigned int i = j; i < s.size(); i++) {
		if (!punto && s[i] == '.') {
			punto = true;
		} else if (s[i] < '0' || s[i] > '9') {
			std::string msg = LocalizationManager::Instance().Translate("La cadena (\\\"%s\\\") no representa un numero.");
			size_t pos = msg.find("%s");
			if (pos != std::string::npos) msg.replace(pos, 2, s);
			err_handler.ErrorIfRunning(311, msg);
			return DataValue::MakeEmpty(vt_numerica);
		}
	}
	return DataValue::MakeReal(s);
}

DataValue func_ftoa(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeString(arg[0].GetForUser());
}

DataValue func_pi(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(3.141592653589793238462643383279502884197169399375105820974944592);
}

DataValue func_euler(ErrorHandler &err_handler, DataValue *arg) {
	return DataValue::MakeReal(2.7182818284590452353602874713527);
}

void FuncsManager::LoadPredefs() {
	m_predefs[LocalizationManager::Instance().Translate("PI")]                 = std::make_unique<Funcion>(vt_numerica, func_pi);
	m_predefs[LocalizationManager::Instance().Translate("EULER")]              = std::make_unique<Funcion>(vt_numerica, func_euler);
	m_predefs[LocalizationManager::Instance().Translate("RC")]                 = std::make_unique<Funcion>(vt_numerica, func_rc, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("RAIZ")]               = std::make_unique<Funcion>(vt_numerica, func_rc, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("ABS")]                = std::make_unique<Funcion>(vt_numerica, func_abs, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("LN")]                 = std::make_unique<Funcion>(vt_numerica, func_ln, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("EXP")]                = std::make_unique<Funcion>(vt_numerica, func_exp, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("SEN")]                = std::make_unique<Funcion>(vt_numerica, func_sen, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("SIN")]                = std::make_unique<Funcion>(vt_numerica, func_sen, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("ASEN")]               = std::make_unique<Funcion>(vt_numerica, func_asen, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("ASIN")]               = std::make_unique<Funcion>(vt_numerica, func_asen, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("ACOS")]               = std::make_unique<Funcion>(vt_numerica, func_acos, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("COS")]                = std::make_unique<Funcion>(vt_numerica, func_cos, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("TAN")]                = std::make_unique<Funcion>(vt_numerica, func_tan, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("ATAN")]               = std::make_unique<Funcion>(vt_numerica, func_atan, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("AZAR")]               = std::make_unique<Funcion>(vt_numerica, func_azar, vt_numerica_entera);
	m_predefs[LocalizationManager::Instance().Translate("ALEATORIO")]          = std::make_unique<Funcion>(vt_numerica, func_aleatorio, vt_numerica_entera, vt_numerica_entera);
	m_predefs[LocalizationManager::Instance().Translate("TRUNC")]              = std::make_unique<Funcion>(vt_numerica, func_trunc, vt_numerica);
	m_predefs[LocalizationManager::Instance().Translate("REDON")]              = std::make_unique<Funcion>(vt_numerica, func_redon, vt_numerica);

	if (lang[LS_ENABLE_STRING_FUNCTIONS]) {
		m_predefs[LocalizationManager::Instance().Translate("CONVERTIRANÚMERO")] = std::make_unique<Funcion>(vt_numerica, func_atof, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("CONVERTIRANUMERO")] = std::make_unique<Funcion>(vt_numerica, func_atof, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("CONVERTIRATEXTO")]  = std::make_unique<Funcion>(vt_caracter, func_ftoa, vt_numerica);
		m_predefs[LocalizationManager::Instance().Translate("LONGITUD")]         = std::make_unique<Funcion>(vt_numerica, func_longitud, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("SUBCADENA")]        = std::make_unique<Funcion>(vt_caracter, func_subcadena, vt_caracter, vt_numerica_entera, vt_numerica_entera);
		m_predefs[LocalizationManager::Instance().Translate("MAYUSCULAS")]       = std::make_unique<Funcion>(vt_caracter, func_mayusculas, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("MINUSCULAS")]       = std::make_unique<Funcion>(vt_caracter, func_minusculas, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("MAYÚSCULAS")]       = std::make_unique<Funcion>(vt_caracter, func_mayusculas, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("MINÚSCULAS")]       = std::make_unique<Funcion>(vt_caracter, func_minusculas, vt_caracter);
		m_predefs[LocalizationManager::Instance().Translate("CONCATENAR")]       = std::make_unique<Funcion>(vt_caracter, func_concatenar, vt_caracter, vt_caracter);
	}

	m_predefs[LocalizationManager::Instance().Translate("FECHAACTUAL")]        = std::make_unique<Funcion>(vt_numerica, func_fecha);
	m_predefs[LocalizationManager::Instance().Translate("HORAACTUAL")]         = std::make_unique<Funcion>(vt_numerica, func_hora);
}