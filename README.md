# ⚡ LightNotes: Ultra Fast & Light

![LightNotes Icono](lightnotes.png)

**El bloc de notas que se carga antes de que pestañees.**

LightNotes es un editor de texto ultraligero y de alto rendimiento para Windows. Desarrollado en C++ puro y WinAPI, está diseñado para ser la alternativa más rápida y eficiente al Bloc de Notas estándar, pero con las características modernas que los usuarios de hoy necesitan.

## ✨ Características Principales

Hemos priorizado la velocidad y las utilidades clave para ofrecer una experiencia de escritura sin fricciones:

* **Rendimiento Extremo:** Archivo ejecutable de solo **33 KB** (¡más ligero que un meme!) y carga instantánea.
* **Modo Oscuro:** Alterna fácilmente a un tema oscuro para reducir la fatiga visual durante la noche.
* **Zoom Dinámico:** Ajusta el tamaño del texto entre **50% y 300%** utilizando atajos de teclado o `Ctrl + Rueda del ratón`.
* **Estadísticas en Vivo:** Consulta al instante el conteo de Palabras, Líneas y Caracteres en la barra de estado.
* **Herramientas de Edición:** Funciones rápidas para convertir texto a MAYÚSCULAS o minúsculas.
* **Compatibilidad:** Soporte robusto para Drag & Drop (arrastrar y soltar) de archivos y detección de codificación UTF-8/UTF-16.
* **Modo Pantalla Completa (`F11`):** Elimina distracciones para concentrarte en el texto.

## ⬇️ Descarga e Instalación

LightNotes es un ejecutable portátil. No requiere instalación.

1.  Ve a la sección **[Releases](https://github.com/andresavalosgallegos-art/LightNotes/releases)** o descarga directamente el archivo.
2.  Descarga el ejecutable: **[lightnotes.exe](lightnotes.exe)** (¡Solo 33 KB!)
3.  Simplemente haz doble clic en `lightnotes.exe` para comenzar a usarlo.

## ⌨️ Atajos de Teclado Útiles

| Atajo | Función |
| :--- | :--- |
| `Ctrl + N` | Nuevo documento |
| `Ctrl + O` | Abrir archivo... |
| `Ctrl + S` | Guardar archivo |
| `Ctrl + F` | Buscar |
| `Ctrl + +` / `Ctrl + -` | Acercar / Alejar Zoom |
| `Ctrl + 0` | Restablecer Zoom al 100% |
| `F5` | Insertar Fecha/Hora actual |
| `F11` | Alternar Pantalla Completa |

## 🛠️ Compilación (Para Desarrolladores)

Este proyecto está construido con C++ y la API de Windows.

### Requisitos
* Compilador MinGW o Visual Studio (que soporte la API de Windows).
* `richedit.h` y librerías asociadas (`riched20.dll`).

### Pasos de Compilación
1.  Clona el repositorio:
    ```bash
    git clone [https://github.com/andresavalosgallegos-art/LightNotes.git](https://github.com/andresavalosgallegos-art/LightNotes.git)
    cd LightNotes
    ```
2.  Compila el archivo fuente `lightnotes.cpp` utilizando tu compilador (ejemplo con g++):
    ```bash
    g++ lightnotes.cpp -o lightnotes.exe -mwindows -lcomctl32 -lcomdlg32 -lshlwapi
    ```

## 👤 Creador

**LightNotes** fue desarrollado con pasión y dedicación por:

**Andrés Santiago Ávalos Gallegos**

---
*(c) 2025 - Light as a feather!*

**[Ir a la Landing Page (GitHub Pages)](https://andresavalosgallegos-art.github.io/LightNotes/)**
