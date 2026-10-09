import pytest
from operaciones import dividir, es_par, aplicar_descuento 

def test_dividir():
    assert dividir(10, 2) == 5.0
    assert dividir(-15, 3) == -5.0
    assert dividir(7, 2) == 3.5

def test_dividir_por_cero():
    with pytest.raises(ValueError, match="No se puede dividir por cero"):
        dividir(10, 0)

def test_es_par_verdadero():
    assert es_par(4) is True
    assert es_par(0) is True      # El cero se considera par
    assert es_par(-8) is True     # Los negativos también pueden ser pares

def test_es_par_falso():
    assert es_par(7) is False
    assert es_par(-3) is False

def test_aplicar_descuento_flotantes():
    pytest.approx(0,1) # compensa los pequeños desajustes de los decimales en programación
    assert aplicar_descuento(100, 15) == pytest.approx(85.0)
    assert aplicar_descuento(19.99, 10) == pytest.approx(17.991)