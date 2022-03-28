# Графические фильтры (image_processor)

В этом задании требуется реализовать консольное приложение,
позволяющее применять к изображениям различные фильтры,
аналогичные фильтрам в популярных графических редакторах.

## Поддерживаемый формат изображений

Входные и выходные графические файлы должны быть в формате [BMP](http://en.wikipedia.org/wiki/BMP_file_format).

Формат BMP поддерживает достаточно много вариаций, но в этом задании будет использоваться
24-битный BMP без сжатия и без таблицы цветов. Тип используемого `DIB header` - `BITMAPINFOHEADER`.

Пример файла в нужном формате есть в статье на Википедии [в разделе "Example 1"](https://en.wikipedia.org/wiki/BMP_file_format#Example_1)
и в папке [examples](examples).

При тестировании обязательно обращайте внимание на то, чтобы тестовое изображение
было сохранено именно в 24-битном BMP.

## Формат аргументов командной строки

Описание формата аргументов командной строки:

`{имя программы} {путь к входному файлу} {путь к выходному файлу}
[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]
[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...`

При запуске без аргументов программа выводит справку.

### Пример
`./image_processor input.bmp /tmp/output.bmp -crop 800 600 -gs -blur 0.5`

В этом примере
1. Загружается изображение из файла `input.bmp`
2. Обрезается до изображения с началом в верхнем левом углу и размером 800х600 пикселей
3. Переводится в оттенки серого
4. Применяется размытие с сигмой 0.5
5. Полученное изображение сохраняется в файл `/tmp/output.bmp`

Список фильтров может быть пуст, тогда изображение должно быть сохранено в неизменном виде.
Фильтры применяются в том порядке, в котором они перечислены в аргументах командной строки.

## Фильтры

В формулах далее считаем, что каждая компонента цвета
представлена вещественным числом от 0 до 1. Цвета пикселей
представлены тройками `(R, G, B)`. Таким образом, `(0, 0, 0)` – черный,
`(1, 1, 1)` – белый.

Если фильтр задан матрицей, это означает, что значение каждого из цветов определяется взвешенной суммой
значений этого цвета в соседних пикселях в соответствии с матрицей. При этом целевому пикселю
соответствует центральный элемент матрицы.

Например, для фильтра, заданного матрицей

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D1%20&%202%20&%203%20%5C%5C4%20&%205%20&%206%20%5C%5C7%20&%208%20&%209%20%5C%5C%5Cend%7Bbmatrix%7D)

Значение каждого из цветов целевого пикселя `C[x][y]` будет определяться формулой

```
C[x][y] =
  min(1, max(0,
   1*C[x-1][y-1] + 2*C[x][y-1] + 3*C[x+1][y-1] +
   4*C[x-1][y]   + 5*C[x][y]   + 6*C[x+1][y]   +
   7*C[x-1][y+1] + 8*C[x][y+1] + 9*C[x+1][y+1]
))
```

При обработке пикселей, близких к краю изображения, часть матрицы может выходить за границу изображения.
В таком случае в качестве значения пикселя, выходящего за границу, следует использовать значение ближайшего
к нему пикселя изображения.

### Список базовых фильтров

#### Crop (-crop width height)
Обрезает изображение до заданных ширины и высоты. Используется верхняя левая часть изображения.

Если запрошенные ширина или высота превышают размеры исходного изображения, выдается доступная часть изображения.

#### Grayscale (-gs)
Преобразует изображение в оттенки серого по формуле

![encoding](https://latex.codecogs.com/svg.image?R'%20=%20G'%20=%20B'%20=0.299%20R%20&plus;%200%20.587%20G%20&plus;%200%20.%20114%20B)

#### Negative (-neg)
Преобразует изображение в негатив по формуле

![encoding](https://latex.codecogs.com/svg.image?R'%20=%201%20-%20R,%20G'%20=%201%20-%20G,%20B'%20=%201%20-%20B)

#### Sharpening (-sharp)
Повышение резкости. Достигается применением матрицы

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%205%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

#### Edge Detection (-edge threshold)
Выделение границ. Изображение переводится в оттенки серого и применяется матрица

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%204%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

Пиксели со значением, превысившим `threshold`, окрашиваются в белый, остальные – в черный.

#### Gaussian Blur (-blur sigma)
[Гауссово размытие](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу),
параметр – сигма.

Значение каждого из цветов пикселя `C[x0][y0]` определяется формулой

![encoding](https://latex.codecogs.com/svg.image?C%5Bx_0%5D%5By_0%5D%20=%20%5Csum_%7Bx=0,y=0%7D%5E%7Bwidth-1,%20height-1%7DC%5Bx%5D%5By%5D%5Cfrac%7B1%7D%7B%5Csqrt%5B%5D%7B2%5Cpi%5Csigma%5E2%7D%7De%5E%7B-%5Cfrac%7B%5Cleft%7Cx_o-x%5Cright%7C%5E2%20&plus;%20%5Cleft%7Cy_o-y%5Cright%7C%5E2%7D%7B2%5Csigma%5E2%7D%7D)

Существуют различные варианты релализации и оптимизации вычисления этого фильтра, описание есть [в Википедии](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу).

### Дополнительные фильтры

Дополнительно надо сделать как минимум один собственный фильтр.

Фильтр должен быть нетривиальным. Идеи можно взять
[здесь](https://developer.apple.com/library/mac/documentation/graphicsimaging/reference/CoreImageFilterReference/Reference/reference.html).

Хорошие примеры: [Crystallize](https://developer.apple.com/library/archive/documentation/GraphicsImaging/Reference/CoreImageFilterReference/index.html#//apple_ref/doc/filter/ci/CICrystallize), [Glass Distortion](https://developer.apple.com/library/archive/documentation/GraphicsImaging/Reference/CoreImageFilterReference/index.html#//apple_ref/doc/filter/ci/CIGlassDistortion).

Выбор собственного фильтра надо согласовать с семинаристом.

Авторы самых интересных (по мнению лектора) фильтров получат бонус к баллам.

## Реализация

Применять сторонние библиотеки для работы с изображениями запрещено.

Старайтесь делать все компоненты программы по возможности более универсальными и не привязанными к специфике конкретной задачи.

Все исключительные ситуации должны корректно обрабатываться с выводом понятного пользователю сообщения об ошибке.
Никакие сценарии, включая использование файлов с форматом, не соответствующим спецификации, не должны приводить к "падению" программы.

Скорее всего, вам понадобятся следующие компоненты:
- Класс, представляющий изображение и обеспечивающий работу с ним
- Классы для чтения и записи формата BMP
- Фильтры
- Контроллер, управляющий последовательным применением фильтров

Общие части следует выделить через наследование.

Подробный дизайн программы рекомендуется обсудить с преподавателем на семинарах.

## Сдача и оценивание

Все сдачи будут проходить код ревью.

Вам предстоит написать всю программу с нуля.
В корневом `CMakeLists.txt` задачи должен быть описан исполняемый файл с именем `image_processor`,
в остальном вы можете менять файлы `CMakeLists.txt` как угодно.

Для отправки кода проекта используйте ветку `projects/image_processor`.

Статус проекта не будет отображаться на сайте https://cpp-hse.ru, поэтому после загрузки решения
вам надо будет самостоятельно создать MR, назначить семинариста ревьюером и попросить его проверить ваш код.

### Принципы начисления баллов

Минимальная реализация проекта оценивается в **3 балла** и должна содержать:
- Класс для представления цвета
- Класс для представления изображения
- Классы или функции для загрузки изображения из формата BMP и сохранения изображения в BMP
- Один класс фильтра

Решения, не удовлетворяющие требованиям к минимальной реализации, оцениваются в 0 баллов.

Если решение удовлетворяет требованиям к минимальной реализации, дополнительно начисляется
0.6 балла за каждый реализованный базовый фильтр (кроме входящего в минимальную реализацию). Таким образом,
за фильтры можно получить еще до **3 баллов**.

Если реализовано не менее 5 фильтров и фильтры образуют полную иерархию наследования,
начисляется еще **2 балла**.

За реализованный дополнительный фильтр начисляется **1 балл**. Дополнительный фильтр можно сдавать,
если реализовано не менее 5 базовых фильтров.

За покрытие юнит тестами всех критичных компонентов начисляется **1 балл**.
Ваш семинарист поможет с определением достаточности покрытия.

Баллы могут быть сняты за серьезные недочеты, например:
- Отсутствие или неверную декомпозицию на файлы, классы, методы и функции
- Утечки памяти
- Падение программы
  - на файлах неверного формата
  - при вызове фильтров с некорректными аргументами
  - при вызове некорректных фильтров
- Неверный результат работы фильтров
- Нечитаемый код
- Нарушение стиля кодирования
- UB на каком-либо входе или наборе аргументов функции
- Выбор неэффективных структур данных или их неэффективное использование
- Лишние копирования объектов
- Неверные сигнатуры методов

## Дедлайн

Для проекта установлен **жесткий** дедлайн `23:59 27.03.2022` - это означает, что решения,
отправленные после этого времени, не будут оцениваться.

Дополнительных штрафов за время сдачи нет.