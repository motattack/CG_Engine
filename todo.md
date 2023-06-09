## Задание 1. Библиотека математики.

Veci, i = {2, 3, 4}

Методы:
- Конструкторы:
  - Veci()
  - Veci(float)m
  - Veci(float, float, ...)
  - Veci(veci)
- сложение векторов
- вычитание векторов
- скалярное произведение
- умножение на число
- деление на число
- векторное произведение (для Vec3)
- длина вектора
- нормализация
- сравнение векторов ==, !=

Mati, i = {2, 3, 4}

Методы:
- Конструкторы:
    - Mati()
    - Mati(float)m
    - Mati(float, float, ...)
    - Mati(Mati)
- сложение матриц
- вычитание матриц
- умножение матриц
- умножение на число
- деление на число
- прибавления числа
- вычитание числа
- умножение матрицы на вектор
- единичная матрица
- обратная матрица
- транспонированная матрица
- сравнение векторов ==, !=
- смещение
- поворот
- масштабирование
- получить строку/столбец

Доп методы
- LookAt
- Perspective
- Ortho

## Задание 2. Обёртки и редактор

Добавить обёртки для opengl:
- vertex buffer
- index buffer
- shader
- texture
- vertex
```cpp
mesh {
    VertexBuffer<Vertex> vBuffer;
    IndexBuffer iBuffer;
}
```

Добавить редактор на imgui:

- меню, где можно добавить примитив (плоскость, куб, сфера)
- список объектов сцены
- виджет изменения позиции масштаба и поворота для выбранного объекта из списка
- виджет материала, где можно выбрать текстуру для выбранного объекта из списка
(если текстура не установлена, то используется какая-нибудь по умолчанию)

## Задание 3. Источники света

- Добавить поддержку источников освещения (directional, point, spot).
Максимальное количество источников на сцене 30 штук.

- Добавить в редактор возможность добавлять к объекту источник света.
Объект может содержать максимум один источник света.

## Задание 4. Расширение функциональности

Материал
- Добавить поддержку normal/parallax map
- Улучшить материал:
    - добавить возможность добавлять карты normal/parallax
    - добавить цвет объекта, тайлинг и сдвиг для текстур.
- Если карта/текстура не установлена - то рендерится с дефолтной

Редактор
- Добавить возможность редактировать новые функции материала для каждого объекта.

- Добавить виджет отображения ресурсов (текстуры), которые можно выбирать и добавлять к объектам.

- Добавить объекты и компоненты для объектов:
    - Объекты можно добавлять/удалять.
    - Объект может хранить переменное число компонент.
    - При создании объекта у него сразу есть трансформ компонента.
    - Компоненты можно добавлять и удалять (кроме трансформы, она всегда есть).

- Виды компонент:
    - Трансформ - позиция, поворот, масштаб, вкл/выкл объект.
  (Если выкл, то он не рендерится и не апдейтится)

  - Модель - позволяет выбрать примитив или модельку.
    - Материал - из редактора настраиваются все юниформы (текстуры, цвет и т.д.)

    - Источники света - редактируются все параметры
    
    - Камеры:
      - Может быть сколько угодно камер
      - Есть главная камера, если она включена, рендер идёт относительно неё.
      - Параметры настраиваются из редактора.
      - Камера не отвечает за перехват событий, свое положение и поворот она берёт от трансформы.

    - Инпут компонент:
      - Если есть, то объект способен получать ивенты с клавиатуры и мыши (если вкл).
      - У инпут компонента есть обработчик для обновления трансформ компоненты.

- Добавить загрузку моделей (В репе есть пример загрузки модельки через либу assimp)

## Задание 5. Frame buffer. Постпроцессинг. SkyBox

- Добавить в текущий рендер frame buffer и перед выводом текстуры на экран добавить постпроцессинг (эффект ч/б).
- Добавить возможность включать/отключать через imgui
- Добавить скай бокс. И возможность его настройки через ImGui.

## Задание на доп балл.
- Добавить SSBO и сделать динамическое количество источников света