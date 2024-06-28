# Transformar MSC23 em sensor de tensão para a bateria

1. Soldar a placa normalmente usando o projeto que esta na branch voltage_sensor, 
não soldar compnentes que estão com X vermelho no esquematico

![alt text](img/supplyNoPopulate.png)
![alt text](img/analogNoPopulate.png)

2. Soldar o conector na camada inversa a do projeto com as duas pernas centrais isoladas e distanciadas dos pads. 
Na proxima imagem tem exemplo, é recomendavel colocar algo para isolar o pino do pad (talvez o fenolite de uma PCB)

![alt text](img/conectorExample.png)

Deberia ficar algo parecido com a imagem:

![alt text](img/connectorExample2.png)

3. No proximo passo serão soldados os componentes selecionados em montagem aerea

![alt text](img/schematicsAnalog.png)

Deveria ficar algo parecido com a imagem:

![alt text](img/resistorExample.png)

# Documentos de Referência
[Simulação](design/analog_voltage_sensor.asc)
[Esquemático](exports/schematic.pdf/)

![Simulação](img/simulation.png)
