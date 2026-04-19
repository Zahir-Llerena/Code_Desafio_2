# UdeAWorldCup - Simulador del Mundial FIFA 2026

[cite_start]Este repositorio contiene la solución al **Desafío II** de la asignatura **Informática II (Semestre 2026-1)** de la Facultad de Ingeniería, Universidad de Antioquia [cite: 296-301]. [cite_start]El objetivo es modelar y simular el torneo mundialista utilizando el paradigma de Programación Orientada a Objetos (POO) en C++ [cite: 325-326].

## 📋 Descripción del Proyecto
[cite_start]El sistema permite la gestión y simulación automatizada de los 104 partidos del mundial [cite: 377][cite_start], basándose en modelos estadísticos de probabilidad de goles ($\lambda$) y rankings oficiales de la FIFA [cite: 383-391].

## 🛠️ Restricciones Técnicas (Core de la Solución)
[cite_start]Para garantizar la eficiencia y el dominio del lenguaje, el desarrollo se ciñe a las siguientes restricciones estrictas [cite: 550-553, 586-589]:
- **Sin STL**: Uso exclusivo de estructuras de datos propias desarrolladas con plantillas (templates).
- **Sin Structs**: Implementación pura mediante clases y encapsulamiento.
- **Memoria Dinámica**: Gestión manual de recursos y punteros.
- [cite_start]**Eficiencia**: Monitoreo en tiempo real de iteraciones y consumo de memoria en bytes [cite: 510-513].

## 🏗️ Arquitectura del Software
La solución se construye de forma modular y ascendente:
1. **Estructuras Base**: Implementación de `Lista<T>` dinámica.
2. **Entidades**: Clases `Equipo`, `Jugador` y `Partido`.
3. **Persistencia**: Lector manual de archivos CSV y generador de reportes TXT.
4. **Motor de Simulación**: Lógica de grupos, eliminatorias y criterios de desempate.
5. **Auditoría**: Sistema de medición de recursos.

## 👤 Autores
**Integrantes del grupo: Clayder Areiza Pino & Arlington Zahir Llerena 
**Email: clayder.areiza@udea.edu.co, zahir.llerena@udea.edu.co
