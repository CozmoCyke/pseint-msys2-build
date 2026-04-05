#include <sstream>
#include "CBOTExporter.hpp"
#include "TiposExporter.hpp"
#include "exportexp.h"

using namespace std;

CBOTExporter::CBOTExporter() : CppExporter() {
	output_base_zero_arrays = true;
}

string CBOTExporter::make_string(string cont) {
	for (unsigned int i=0; i<cont.size(); ++i) {
		if (cont[i]=='\\' || cont[i]=='"') cont.insert(i++,"\\");
	}
	return string("\"")+cont+"\"";
}

string CBOTExporter::get_constante(string name) {
	if (name=="VERDADERO") return "true";
	if (name=="FALSO") return "false";
	if (name=="PI") return "3.14159265";
	return name;
}

string CBOTExporter::get_operator(string op, bool for_string) {
	(void)for_string;
	if (op=="(") return "(";
	if (op==")") return ")";
	if (op=="{") return "(";
	if (op=="}") return ")";
	if (op=="[") return "[";
	if (op==",") return "][";
	if (op=="]") return "]";
	if (op=="+") return "+";
	if (op=="-") return "-";
	if (op=="*") return "*";
	if (op=="/") return "/";
	if (op=="^") return "func pow(arg1, arg2)";
	if (op=="%") return "%";
	if (op=="=") return "==";
	if (op=="<>") return "!=";
	if (op=="<") return "<";
	if (op==">") return ">";
	if (op=="<=") return "<=";
	if (op==">=") return ">=";
	if (op=="&") return " && ";
	if (op=="|") return " || ";
	if (op=="~") return "!";
	return op;
}

string CBOTExporter::function(string name, string args) {
	if (name=="SEN") return string("sin")+args;
	if (name=="COS") return string("cos")+args;
	if (name=="TAN") return string("tan")+args;
	if (name=="ASEN") return string("asin")+args;
	if (name=="ACOS") return string("acos")+args;
	if (name=="ATAN") return string("atan")+args;
	if (name=="RAIZ" || name=="RC") return string("sqrt")+args;
	if (name=="TRUNC") return string("trunc")+args;
	if (name=="REDON") return string("round")+args;
	if (name=="AZAR") return string("trunc(rand() * ")+colocarParentesis(get_arg(args,1))+")";
	if (name=="ALEATORIO") {
		string a = get_arg(args,1);
		string b = get_arg(args,2);
		return string("(")+a+" + trunc(rand() * (("+b+")-("+a+")+1)))";
	}
	if (name=="CONCATENAR") return string("(")+get_arg(args,1)+"+"+get_arg(args,2)+")";
	if (name=="LONGITUD") return string("strlen")+args;
	if (name=="SUBCADENA") {
		string desde = get_arg(args,2);
		if (!input_base_zero_arrays) desde = sumarOrestarUno(desde,false);
		string cuantos = sumarOrestarUno(get_arg(args,3)+"-"+get_arg(args,2),true);
		return string("strmid(")+get_arg(args,1)+", "+desde+", "+cuantos+")";
	}
	if (name=="MAYUSCULAS") return string("strupper")+args;
	if (name=="MINUSCULAS") return string("strlower")+args;
	if (name=="CONVERTIRANUMERO") return string("strval")+args;
	if (name=="CONVERTIRATEXTO") return string("(\"\" + ")+get_arg(args,1)+")";
	if (name=="ABS") {
		warn_abs = true;
		string x = get_arg(args,1);
		return string("((")+x+") < 0 ? -(("+x+")) : ("+x+"))";
	}
	if (name=="LN") {
		warn_ln = true;
		return string("(0 /* TODO: LN sin equivalente CBOT confirmado */)");
	}
	if (name=="EXP") {
		warn_exp = true;
		return string("(0 /* TODO: EXP sin equivalente CBOT confirmado */)");
	}
	return ToLower(name)+args;
}

string CBOTExporter::get_tipo(map<string,tipo_var>::iterator &mit, bool for_func, bool by_ref) {
	tipo_var &t = mit->second;
	string stipo = "int ";
	if (t==vt_caracter) stipo = "string ";
	else if (t==vt_numerica) stipo = t.rounded ? "int " : "float ";
	else if (t==vt_logica) stipo = "bool ";
	else use_sin_tipo = true;

	if (t.dims) {
		return stipo+ToLower(mit->first)+make_dims(t.dims,"[","][","]",!for_func);
	}

	if (by_ref) warn_scalar_reference = true;
	return stipo+ToLower(mit->first);
}

string CBOTExporter::get_tipo(string name, bool by_ref, bool do_erase) {
	map<string,tipo_var>::iterator mit = memoria->GetVarInfo().find(name);
	if (mit==memoria->GetVarInfo().end()) return "int "+ToLower(name);
	string ret = get_tipo(mit,true,by_ref);
	if (do_erase) memoria->GetVarInfo().erase(mit);
	return ret;
}

void CBOTExporter::declarar_variables(t_output &prog, string tab, bool ignore_arrays) {
	map<string,tipo_var>::iterator mit = memoria->GetVarInfo().begin(), end = memoria->GetVarInfo().end();
	while (mit!=end) {
		if (!ignore_arrays || !mit->second.dims) prog.push_back(tab+get_tipo(mit)+";");
		++mit;
	}
}

void CBOTExporter::header(t_output &out) {
	init_header(out,"// ");
	out.push_back("// Exportador CBOT V3: soporte prudente para arreglos, con limites explicitados en comentarios.");
	if (warn_scalar_reference) out.push_back("// ADVERTENCIA: por referencia en escalares no esta soportado de forma exacta en CBOT.");
	if (warn_leer) out.push_back("// ADVERTENCIA: Leer no tiene equivalente interactivo documentado en CBOT.");
	if (warn_esperar_tecla) out.push_back("// ADVERTENCIA: EsperarTecla no tiene equivalente documentado en CBOT.");
	if (warn_borrar_pantalla) out.push_back("// ADVERTENCIA: BorrarPantalla no tiene equivalente documentado en CBOT.");
	if (warn_paracada) out.push_back("// ADVERTENCIA: ParaCada tiene soporte parcial y algunos casos quedan sin traducir.");
	if (warn_paracada_multidim) out.push_back("// ADVERTENCIA: ParaCada solo se reescribe automaticamente para arreglos CBOT de una dimension.");
	if (warn_dimension_runtime) out.push_back("// ADVERTENCIA: Dimension se refleja sobre todo en la declaracion del arreglo; la materializacion runtime exacta no se fuerza en V3.");
	if (warn_dimension_multidim) out.push_back("// ADVERTENCIA: los arreglos multidimensionales se declaran, pero su materializacion exacta en runtime queda diferida.");
	if (warn_redimension_1d) out.push_back("// ADVERTENCIA: Redimensionar 1D no se traduce como operacion runtime; la declaracion final solo refleja el ultimo tamano observado.");
	if (warn_redimension_multidim) out.push_back("// ADVERTENCIA: Redimensionar multidimensional no esta soportado en CBOTExporter V3.");
	if (warn_sinsaltar) out.push_back("// ADVERTENCIA: Escribir Sin Saltar se exporta usando message(...) y pierde la semantica de flujo.");
	if (warn_abs) out.push_back("// ADVERTENCIA: ABS se exporta con una expresion inline por falta de confirmacion en la documentacion local.");
	if (warn_ln) out.push_back("// ADVERTENCIA: LN no tiene equivalente CBOT confirmado en la documentacion local.");
	if (warn_exp) out.push_back("// ADVERTENCIA: EXP no tiene equivalente CBOT confirmado en la documentacion local.");
	out.push_back("");
}

void CBOTExporter::footer(t_output &out) {
	(void)out;
}

void CBOTExporter::dimension(t_output &prog, t_arglist &nombres, t_arglist &tamanios, std::string tabs) {
	ExporterBase::dimension(prog,nombres,tamanios,tabs);
	for (size_t i=0; i<nombres.size(); ++i) {
		string arr = ToLower(nombres[i]);
		const int *dims = memoria->LeerDims(arr);
		if (!dims) {
			insertar(prog,tabs+"// ADVERTENCIA: no se pudieron determinar las dimensiones de "+arr+".");
			continue;
		}
		if (current_dimension_is_redimension) {
			if (dims[0]==1) {
				warn_redimension_1d = true;
				insertar(prog,tabs+"// Redimensionar("+arr+"): la declaracion final reflejara el ultimo tamano observado.");
				insertar(prog,tabs+"// ADVERTENCIA: no se genera una operacion runtime real de redimensionamiento en CBOT.");
			} else {
				warn_redimension_multidim = true;
				insertar(prog,tabs+"// Redimensionar("+arr+"): caso multidimensional detectado.");
				insertar(prog,tabs+"// ADVERTENCIA: Redimensionar arreglos multidimensionales no esta soportado en CBOTExporter V3.");
			}
		} else if (dims[0]==1) {
			warn_dimension_runtime = true;
			insertar(prog,tabs+"// Dimension("+arr+"): se exporta principalmente en la declaracion del arreglo.");
			insertar(prog,tabs+"// ADVERTENCIA: CBOT materializa el arreglo al primer acceso o asignacion; V3 no fuerza esa inicializacion.");
		} else {
			warn_dimension_runtime = true;
			warn_dimension_multidim = true;
			insertar(prog,tabs+"// Dimension("+arr+"): las dimensiones quedan reflejadas en la declaracion.");
			insertar(prog,tabs+"// ADVERTENCIA: la materializacion exacta de arreglos multidimensionales no esta garantizada en V3.");
		}
	}
}

void CBOTExporter::esperar_tiempo(t_output &prog, string tiempo, bool mili, std::string tabs) {
	tipo_var t;
	tiempo = expresion(tiempo,t);
	if (mili) insertar(prog,tabs+"wait(("+tiempo+")/1000.0);");
	else insertar(prog,tabs+"wait("+tiempo+");");
}

void CBOTExporter::esperar_tecla(t_output &prog, std::string tabs) {
	warn_esperar_tecla = true;
	insertar(prog,tabs+"// ADVERTENCIA: EsperarTecla no soportado en CBOT.");
}

void CBOTExporter::borrar_pantalla(t_output &prog, std::string tabs) {
	warn_borrar_pantalla = true;
	insertar(prog,tabs+"// ADVERTENCIA: BorrarPantalla no soportado en CBOT.");
}

void CBOTExporter::escribir(t_output &prog, t_arglist args, bool saltar, std::string tabs) {
	string linea = "\"\"";
	for (t_arglist_it it=args.begin(); it!=args.end(); ++it) {
		tipo_var t;
		linea += " + ("+expresion(*it,t)+")";
	}
	insertar(prog,tabs+"message("+linea+");");
	if (!saltar) warn_sinsaltar = true;
}

void CBOTExporter::leer(t_output &prog, t_arglist args, std::string tabs) {
	warn_leer = true;
	insertar(prog,tabs+"// ADVERTENCIA: Leer no soportado en CBOT; se asignan valores neutros.");
	for (t_arglist_it it=args.begin(); it!=args.end(); ++it) {
		tipo_var t;
		string var = expresion(*it,t);
		string value = "0";
		if (t==vt_caracter) value = "\"\"";
		else if (t==vt_logica) value = "false";
		else if (t==vt_numerica && !t.rounded) value = "0.0";
		insertar(prog,tabs+var+" = "+value+";");
	}
}

void CBOTExporter::repetir(t_output &prog, t_proceso_it it_repetir, t_proceso_it it_hasta, std::string tabs) {
	auto impl = getImpl<IT_HASTAQUE>(*it_hasta);
	insertar(prog,tabs+"do {");
	bloque(prog,++it_repetir,it_hasta,tabs+"\t");
	if (impl.mientras_que) insertar(prog,tabs+"} while ("+expresion(impl.condicion)+");");
	else insertar(prog,tabs+"} while ("+expresion(invert_expresion(impl.condicion))+");");
}

void CBOTExporter::paracada(t_output &prog, t_proceso_it it_para, t_proceso_it it_fin, std::string tabs) {
	auto &impl = getImpl<IT_PARACADA>(*it_para);
	string identif = expresion(impl.identificador);
	string arreglo = expresion(impl.arreglo);
	const int *dims = memoria->LeerDims(impl.arreglo);
	if (!dims) {
		warn_paracada = true;
		insertar(prog,tabs+"// ADVERTENCIA: ParaCada requiere un arreglo; "+ToLower(impl.arreglo)+" no pudo resolverse como tal.");
		return;
	}
	if (dims[0]!=1) {
		warn_paracada = true;
		warn_paracada_multidim = true;
		insertar(prog,tabs+"// ADVERTENCIA: ParaCada solo se soporta en V2 para arreglos de una dimension.");
		insertar(prog,tabs+"// ADVERTENCIA: caso omitido para "+ToLower(impl.arreglo)+".");
		return;
	}
	string idx = get_aux_varname("cbot_idx_");
	insertar(prog,tabs+"for (int "+idx+" = 0; "+idx+" < sizeof("+arreglo+"); ++"+idx+") {");
	insertar(prog,tabs+"\t"+identif+" = "+arreglo+"["+idx+"];");
	bloque(prog,std::next(it_para),it_fin,tabs+"\t");
	insertar(prog,tabs+"}");
	release_aux_varname(idx);
}

void CBOTExporter::translate_single_proc(t_output &out, Funcion *f, t_proceso &proc) {
	t_output out_proc;
	bloque(out_proc,++proc.begin(),proc.end(),"\t");

	string ret;
	if (!f) {
		string main_name = ToLower(GetRT().funcs.GetMainName());
		out.push_back("extern void object::"+main_name+"() {");
	} else {
		string dec;
		if (f->nombres[0].empty()) dec = "void ";
		else {
			ret = get_tipo(f->nombres[0],false,false);
			dec = ret.substr(0,ret.find(" ")+1);
			ret = string("return")+ret.substr(ret.find(" "));
		}
		dec += ToLower(f->id)+"(";
		for (int i=1; i<=f->GetArgsCount(); ++i) {
			if (i!=1) dec += ", ";
			dec += get_tipo(f->nombres[i],f->pasajes[i]==PP_REFERENCIA);
		}
		dec += ")";
		out.push_back(dec+" {");
	}

	declarar_variables(out);
	insertar_out(out,out_proc);
	if (!ret.empty()) out.push_back(string("\t")+ret+";");
	out.push_back("}");
	if (!for_test) out.push_back("");
}

void CBOTExporter::translate(t_output &out, Programa &prog) {
	load_subs_in_funcs_manager(prog);
	TiposExporter(prog,false);

	t_output out_main, out_procs;
	ExporterBase::translate_all_procs(out_main,out_procs,prog);

	header(out);
	insertar_out(out,out_procs);
	insertar_out(out,out_main);
	footer(out);
}
