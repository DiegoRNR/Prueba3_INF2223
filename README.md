# Prueba Módulo 3 INF2223 PUCV  
## zCode:  
  Xavier Montoya Schlechter  
  Diego Negrín Ramírez  
  Ignacio Córdova Moraga  

## Desafío

La empresa FarmaSalud es una cadena de farmacias con presencia en todo el país. Cuentan con un amplio catálogo de medicamentos y productos de cuidado personal, llegando en algunos casos a manear más de 25.000 productos diferentes, cada uno identificado por un código único de 10 dígitos.
FarmaSalud tiene una red de farmacias distribuidas en diferentes ciudades y regiones. Cada farmacia tiene su propio inventario y capacidad de almacenamiento, que varía según la ubicación y el tamaño de la tienda. En promedio, cada farmacia puede almacenar hasta 10,000 unidades de productos.
La empresa se enfrenta a diversos desafíos en la gestión de su inventario:
1. Control de lotes y fechas de caducidad: Muchos medicamentos tienen una vida útil limitada y deben ser vendidos antes de su fecha de caducidad. Es crucial llevar un registro de los lotes y fechas de caducidad para cada producto en cada farmacia.
2. Distribución óptima de productos: FarmaSalud debe asegurarse de que cada farmacia tenga el stock adecuado de los productos más demandados según su ubicación y clientela. Esto requiere un análisis continuo de datos de ventas y una distribución eficiente de productos entre las farmacias.
3. Gestión de productos controlados: Algunos medicamentos requieren receta médica y están sujetos a controles legales especiales. La aplicación debe garantizar el cumplimiento de estos requisitos y mantener un registro detallado de la venta de productos controlados.
4. Abastecimiento y relaciones con proveedores: FarmaSalud trabaja con múltiples proveedores y fabricantes de medicamentos. La aplicación debe facilitar la generación de órdenes de compra, el seguimiento de envíos y la actualización del inventario cuando se reciben nuevos productos.
5. Alertas de desabastecimiento: La aplicación debe generar alertas cuando el stock de un producto esté por debajo de un umbral predefinido, considerando el promedio de ventas y el tiempo de reabastecimiento.
6. Análisis de datos: FarmaSalud requiere informes y análisis de datos avanzados para identificar tendencias de ventas, patrones estacionales y productos de alta o baja rotación. Esto ayudará a optimizar el inventario y tomar decisiones estratégicas.

Para abordar estos desafíos, se propone desarrollar una aplicación de gestión de inventario utilizando estructuras de datos en C. La aplicación debe incluir las siguientes funcionalidades:
1. Registro de productos con información detallada, incluyendo código, nombre, descripción, categoría, precio, proveedor, lote y fecha de caducidad.
2. Gestión de múltiples farmacias y sus respectivos inventarios.
3. Control de stock en tiempo real para cada producto en cada farmacia.
4. Registro de ventas y actualización automática del inventario.
5. Generación de alertas de caducidad próxima y desabastecimiento.
6. Control especial para productos que requieren receta médica.
7. Generación de órdenes de compra a proveedores basadas en niveles de stock y demanda.
8. Análisis de datos y generación de informes de ventas, productos más vendidos, tendencias estacionales, etc.

Para implementar estas funcionalidades de manera eficiente, se utilizarán estructuras de datos como arrays, listas enlazadas, árboles binarios de búsqueda. Estas estructuras permitirán un rápido acceso y búsqueda de información, así como una gestión optimizada del inventario. Además, se implementarán algoritmos de ordenamiento y búsqueda para facilitar la generación de informes y análisis de datos.

En resumen, FarmaSalud requiere una aplicación robusta de gestión de inventario desarrollada en C que pueda manejar de manera eficiente su amplio catálogo de productos, controlar fechas de caducidad, optimizar la distribución en múltiples farmacias, generar alertas de desabastecimiento, cumplir con regulaciones para productos controlados y proporcionar análisis de datos para la toma de decisiones estratégicas. La implementación adecuada de estructuras de datos y algoritmos será fundamental para el éxito de esta aplicación en el ámbito farmacéutico.
