# Исследование алгоритмов MERGE SORT и MERGE+INSERTION SORT

## Описание задачи

В рамках работы выполнено экспериментальное исследование двух алгоритмов сортировки:
1. **MERGE SORT** — стандартная рекурсивная реализация с выделением дополнительной памяти.
2. **MERGE+INSERTION SORT** — гибридная реализация, использующая INSERTION SORT для массивов малого размера.

### Цель
Сравнить производительность алгоритмов на массивах различного размера и структуры, определить параметры, при которых гибридный алгоритм оказывается более эффективным, а также провести анализ времени работы с использованием библиотеки `std::chrono`.

---

## Подготовка тестовых данных

### Реализация класса `ArrayGenerator`
Для генерации массивов был написан класс `ArrayGenerator`, позволяющий создавать три типа массивов:
1. **Случайные значения** — элементы массива находятся в диапазоне `[0, 6000]`.
2. **Обратный порядок** — элементы массива отсортированы по убыванию.
3. **Почти отсортированные массивы** — некоторое количество случайных пар элементов переставлено местами.

**Параметры тестовых данных:**
- Размеры массивов: от 5000 до 100000 элементов с шагом 1000 (увеличил исходные данные в 10 раз и замеры проводил в наносекундах в связи с быстрой работой моего компьютера, так как на меньших данных и единицах измерения времени появлялись результаты с времененем выполнения 0).
- Массивы берутся разной длины от 5000 до 100000 с шагом 1000.
- Диапазон случайных значений взят от 0 до 6000.

---

## Эмпирический анализ стандартного алгоритма MERGE SORT

### Методика
Для анализа времени работы алгоритма использовалась библиотека `std::chrono`. Время замеров усредняется по 10 запускам для каждого размера массива. 

## Эмпирический анализ гибридного алгоритма MERGE+INSERTION SORT

### Реализация гибридного алгоритма
В гибридной реализации на массивах малого размера (определяемых параметром `threshold`) используется INSERTION SORT. Для анализа рассмотрены значения `threshold`: 5, 10, 20, 30, 50. Мной был выбран параметр = 15.

### Методика
Аналогична методике для MERGE SORT: усреднение времени выполнения на 10 запусках для каждого размера массива и категории тестовых данных.

## Результаты
Графики времени выполнения MERGE SORT и MERGE+INSERTION SORT для всех категорий массивов:

1. **Случайные массивы**  
   ![1](Random%20Array.png)

2. **Обратный порядок**  
   ![2](Reversed%20Array.png)

3. **Почти отсортированные массивы**  
   ![3](Near%20Sorted%20Array.png)

---

## Сравнительный анализ

### Итоги
- **Пороговый параметр `threshold`:** Для большинства тестов оптимальным значением оказалось `threshold = 20`, начиная с которого MERGE+INSERTION SORT работает медленнее стандартной реализации MERGE SORT. 
- **Производительность:** Гибридный алгоритм показывает себя лучше на всех массивах данных, за исключением работы на меньшем количестве данных, отсортированных в обратном порядке.
- **Категории массивов:**
  - Для случайных массивов гибридный алгоритм превосходит стандартный для всех `N`.
  - Для отсортированных в обратном порядке массивов гибридный алгоритм **хуже** стандартного (на маленьких значениях `N`), но **лучше** стандартного (на больших значениях `N`).
  - Для массивов, отсортированных частично, гибридный алгоритм превосходит стандартный для всех `N`.

### Выводы
1. Гибридный алгоритм эффективно снижает время выполнения.
2. Выбор оптимального `threshold` зависит от структуры данных. Для универсального случая значением по умолчанию можно считать `threshold = 20`.

---

## Приложения

### Файлы

#### С данными
- `m_s_test1.txt`
- `m_s_test2.txt`
- `m_s_test3.txt`
- `m_s_i_test1.txt`
- `m_s_i_test2.txt`
- `m_s_i_test3.txt`

#### Графики
- `Random Array.png`
- `Reversed Array.png`
- `Near Sorted Array.png`

#### Файлы с программами
- `main.cpp` (основная логика сортировок + генерация даных)
- `analysis.py` (построение графиков на данных)

### Также реализован был класс SortTester, включающий функции эмпирического замера времени работы рассматриваемых алгоритмов сортировки.

### Ссылка на CodeForces
ID посылки задачи A2i: **[291351932]**

### Ссылка на репозиторий
[GitHub репозиторий с исходными данными и реализациями](https://github.com/Davonchik/Algo_HW_A2)
