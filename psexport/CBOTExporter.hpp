#ifndef CBOT_EXPORTER_HPP
#define CBOT_EXPORTER_HPP

#include "CppExporter.hpp"

class CBOTExporter : public CppExporter {

protected:

	bool warn_leer = false;
	bool warn_esperar_tecla = false;
	bool warn_borrar_pantalla = false;
	bool warn_paracada = false;
	bool warn_scalar_reference = false;
	bool warn_abs = false;
	bool warn_ln = false;
	bool warn_exp = false;
	bool warn_sinsaltar = false;
	bool warn_dimension_runtime = false;
	bool warn_dimension_multidim = false;
	bool warn_paracada_multidim = false;
	bool warn_redimension_1d = false;
	bool warn_redimension_multidim = false;

	virtual std::string get_tipo(std::map<std::string,tipo_var>::iterator &mit, bool for_func=false, bool by_ref=false) override;
	virtual std::string get_tipo(std::string name, bool by_ref=false, bool do_erase=true) override;
	virtual void declarar_variables(t_output &prog, std::string tab="\t", bool ignore_arrays=false) override;
	virtual void header(t_output &out) override;
	virtual void footer(t_output &out) override;
	virtual void translate_single_proc(t_output &out, Funcion *f, t_proceso &proc) override;

	virtual void dimension(t_output &prog, t_arglist &nombres, t_arglist &tamanios, std::string tabs) override;
	virtual void esperar_tiempo(t_output &prog, std::string tiempo, bool mili, std::string tabs) override;
	virtual void esperar_tecla(t_output &prog, std::string tabs) override;
	virtual void borrar_pantalla(t_output &prog, std::string tabs) override;
	virtual void escribir(t_output &prog, t_arglist args, bool saltar, std::string tabs) override;
	virtual void leer(t_output &prog, t_arglist args, std::string tabs) override;
	virtual void repetir(t_output &prog, t_proceso_it it_repetir, t_proceso_it it_hasta, std::string tabs) override;
	virtual void paracada(t_output &prog, t_proceso_it it_para, t_proceso_it it_fin, std::string tabs) override;

public:

	virtual std::string make_string(std::string cont) override;
	virtual std::string function(std::string name, std::string args) override;
	virtual std::string get_constante(std::string name) override;
	virtual std::string get_operator(std::string op, bool for_string=false) override;
	virtual void translate(t_output &out, Programa &prog) override;

	CBOTExporter();
};

#endif
