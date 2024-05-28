# Metaheurísticas - Práctica 2

Este proyecto implementa y evalúa diversas metaheurísticas utilizando validación cruzada de 5 particiones (5CV) para su análisis y comparación. En particular, se evaluan los desempeños de los algoritmos de Búsqueda Aleatoria, RELIEF y Búsqueda Local, además de diversas versiones de algoritmos genéticos y meméticos, en el conocido problema de Apredizaje de Pesos en Características (APC). Nosotros emplearemos la generación de pesos para el algoritmo de clasificación del vecino más cercano (1NN). Los conjuntos de datos utilizados para la evaluación son `breast-cancer`, `ecoli` y `parkinsons`.

## Estructura del Directorio

El proyecto está estructurado de la siguiente manera:

- `/bin`: Contiene el ejecutable `metaheuristics` del proyecto.
- `/data`: Incluye los ficheros de datos ARFF para los conjuntos de datos de breast-cancer, ecoli y parkinsons, así como las tablas APC para el año 2023-24.
- `/docs`: Documentación relacionada, incluyendo el guion del proyecto.
- `/include`: Cabeceras de clases y utilidades empleadas en el proyecto.
- `/obj`: Objetos compilados del proyecto.
- `/outputs`: Resultados de ejecución, soluciones generadas y registros de fitness durante el entrenamiento.
- `/scripts`: Scripts útiles como `run.sh` para la ejecución y `generate_training_fitness_table.py` para la generación de gráficos.
- `/src`: Código fuente del proyecto.
- `/tests`: Pruebas unitarias y mocks utilizados para testing.
- `Makefile`: Archivo para la compilación del proyecto.

## Compilación y Ejecución

Para compilar el proyecto, utiliza el comando `make` en la raíz del proyecto. Esto generará el ejecutable en `/bin`.

Para ejecutar el programa, utiliza el script `run.sh` proporcionado en `/scripts`, especificando los parámetros deseados, por ejemplo:

```shell
./scripts/run.sh --algorithm=local-search --dataset=ecoli --seed=123 --log
```

Esto ejecutará el algoritmo de Búsqueda Local en el conjunto de datos de ecoli con una semilla de 123, registrando los resultados.

## Parámetros de Ejecución

Los programas admiten varios parámetros para personalizar la ejecución, incluidos el algoritmo, el conjunto de datos, la semilla, la opción de registro y diversas combinaciones de parámetros en función de la ejecución deseada.

El programa acepta parámetros de la siguiente manera:

```shell
./scripts/run.sh [--algorithm=ALGORITHM_NAME [--param1=value1 ...]] [--dataset=DATASET_NAME] [--seed=SEED_VALUE] [--log]
```

El orden de los parámetros no importa, pero los valores de los parámetros deben ser especificados en el formato `--param1=value1`. Si bien ninguno de los parámetros es obligatorio, el comportamiento del programa difiere según los parámetros especificados. A continuación, se describen los parámetros admitidos:

1. `--algorithm`: Nombre del algoritmo a ejecutar. Los valores posibles son `1nn`, `relief`, `local-search`,  `best-local-search`, `age-ca`, `age-blx`, `agg-ca`, `agg-blx`, `memetic` o `memetic-restart`.
2. `--dataset`: Nombre del conjunto de datos a utilizar. Los valores posibles son `breast-cancer`, `ecoli` y `parkinsons`.
3. `--seed`: Valor de la semilla a utilizar para la generación de números aleatorios.
4. `--log`: Opción para registrar los resultados de la ejecución.
5. `--param1=value1 ...`: Parámetros adicionales específicos del algoritmo a ejecutar. Consulta la documentación de cada algoritmo para obtener más información.

La ejecución del programa sin parámetros ejecutará la validación cruzada de 5 particiones para los algoritmos de Búsqueda Aleatoria, RELIEF y Búsqueda Local en los conjuntos de datos de `breast-cancer`, `ecoli` y `parkinsons`. El programa mostrará un mensaje de confirmación para confirmar la ejecución. En caso de que no se especifique el conjunto de datos, el programa ejecutará la validación cruzada para todos los conjuntos de datos disponibles. De manera similar, si no se especifica el algoritmo, el programa ejecutará la validación cruzada para todos los algoritmos disponibles. En caso de que no se especifique la semilla, el programa utilizará una semilla aleatoria obtenida entre 0 y 1000.

En cuanto a los parámetros adicionales, de momento tan solo el algoritmo de Búsqueda Local los admite. Para este algoritmo, se pueden especificar los siguientes parámetros:

- `--maxEvaluations`: Número máximo de evaluaciones a realizar. Por defecto, su valor es 15000.
- `--maxNeighbors`: Número máximo de vecinos a explorar en cada iteración. Por defecto, su valor es 20.
- `--variance`: Varianza de la distribución normal para la generación de vecinos. Por defecto, su valor es 0'3.

La implementación alternativa de Búsqueda local admite los mismos parámetros que la implementación original.

- Por su lado, cualquier instancia de una clase hija de `GeneticAlgortihm` acepta los siguientes
  parámetros:
  `maxEvaluations`: Número máximo de evaluaciones de la función objetivo. Por defecto 15000.
- `populationSize`: Tamaño de la población. Por defecto 50.
- `crossoverRate`: Probabilidad de cruce. Por defecto 1, 0.
- `mutationRate`: Probabilidad de mutación. Por defecto 0, 08.

Finalmente, `memetic` y `memetic-restart` aceptan:

- `maxEvaluations`: Número máximo de evaluaciones de la función objetivo. Por defecto 15000.
- `optimizationFrequency`: Frecuencia en generaciones (que no evaluaciones) de optimización. Por defecto 10.
- `selectionRate`: Porcentaje de la población a optimizar. Por defecto 0, 1.
- `elitismRate`: Porcentaje de la selección a optimizar que debe ser de los mejores. Por defecto 0, 0.