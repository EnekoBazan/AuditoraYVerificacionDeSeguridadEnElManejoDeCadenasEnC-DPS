def dividir(a, b):
    if b == 0:
        raise ValueError("No se puede dividir por cero.")
    return a / b

def es_par(numero):
    return numero % 2 == 0

def aplicar_descuento(precio, porcentaje):
    if precio < 0 or porcentaje < 0:
        raise ValueError("Los valores no pueden ser negativos")
    return precio - (precio * (porcentaje / 100))