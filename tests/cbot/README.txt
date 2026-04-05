CBOT export tests for CBOTExporter V2

These tests are small, readable PSeInt programs intended to validate the
current V2 behavior before moving to V3.

Files:

01_variables_asignacion.pseint
Checks simple variables, inferred CBOT types, and assignments.

02_escribir.pseint
Checks Escribir mapped to message(...).

03_dimension_1d.pseint
Checks 1D array declaration plus Dimension warnings.

04_paracada_1d.pseint
Checks ParaCada rewritten as an indexed for loop for a 1D array.

05_paracada_2d_no_soportado.pseint
Checks multidimensional ParaCada is left unsupported with explicit warnings.

06_acceso_2d.pseint
Checks multidimensional array declaration and direct 2D access.

07_dimension_2d_escribir.pseint
Checks 2D Dimension plus writes and Escribir on selected elements.

08_redimension_1d_no_soportado.pseint
Checks Redimensionar 1D stays warning-only and does not pretend to perform runtime resizing.

09_redimension_2d_no_soportado.pseint
Checks Redimensionar multidimensional is left explicitly unsupported.

10_subindice_expresion.pseint
Checks multidimensional indexing with index expressions.

The files under expected/ are approximate expected CBOT outputs.
They are intentionally simple and may differ in comments or minor formatting
while CBOTExporter V2 is still evolving.
