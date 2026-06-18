# sound_processor

`sound_processor` — консольная программа для чтения, генерации, обработки и записи
моно WAV-файлов в формате PCM 16-bit, 44100 Hz.

## Сборка

Из директории `tasks/sound_processor`:

```bash
cmake -S . -B build
cmake --build build
```

Тесты запускаются из директории `build`:

```bash
ctest --output-on-failure
```

## Запуск

```bash
./build/sound_processor [-i input.wav] [-o output.wav] [-f filter_name [params...]]...
```

Параметры:

- `-i input.wav` — входной WAV-файл.
- `-o output.wav` — выходной WAV-файл.
- `-f filter_name [params...]` — фильтр и его параметры. Фильтров может быть несколько, они применяются в порядке перечисления.

Входной и выходной файлы должны иметь расширение `.wav`. Относительные пути
разрешаются относительно текущей рабочей директории, из которой запущена
программа.

Примеры:

```bash
./build/sound_processor -f generator sin 440 1000 -o Output.wav
./build/sound_processor -i Input.wav -f ampl 0.5 -o Output.wav
./build/sound_processor -i Input.wav -f normalize -f lowpass 5 -o Output.wav
```

## CLI-фильтры

### `ampl <factor>`

Умножает каждый сэмпл на коэффициент `factor`.

- `factor` должен быть неотрицательным.
- Результат ограничивается диапазоном `int16_t`.

Пример:

```bash
./build/sound_processor -i Input.wav -f ampl 2.0 -o Output.wav
```

### `normalize [peak]`

Нормализует сигнал так, чтобы максимальная амплитуда стала равна `peak` от
максимума `int16_t`.

- `peak` необязателен, значение по умолчанию — `1.0`.
- `peak` должен быть в диапазоне `[0, 1]`.
- Для пустого или полностью нулевого сигнала фильтр ничего не меняет.

Пример:

```bash
./build/sound_processor -i Input.wav -f normalize 0.8 -o Output.wav
```

### `silence <unit> <start> <end>`

Вставляет нулевые сэмплы в указанный временной диапазон.

- `unit` может быть `sec` или `ms`.
- `start` должен быть не меньше `0`.
- `end` должен быть не меньше `start`.

Пример:

```bash
./build/sound_processor -i Input.wav -f silence ms 500 1000 -o Output.wav
```

### `timestretch <factor>`

Изменяет длительность сигнала с линейной интерполяцией.

- `factor` должен быть положительным.
- `factor > 1` увеличивает длительность.
- `factor < 1` уменьшает длительность.

Пример:

```bash
./build/sound_processor -i Input.wav -f timestretch 1.5 -o Output.wav
```

### `lowpass <window_size>`

Сглаживает сигнал скользящим средним.

- `window_size` должен быть нечетным числом и не меньше `1`.
- На краях сигнала используются ближайшие доступные сэмплы.

Пример:

```bash
./build/sound_processor -i Input.wav -f lowpass 5 -o Output.wav
```

### `generator sin <frequency_hz> <duration_ms>`

Создает синусоидальный сигнал и заменяет им текущий сигнал.

- `frequency_hz` должен быть неотрицательным.
- `duration_ms` должен быть неотрицательным.

Пример:

```bash
./build/sound_processor -f generator sin 440 1000 -o Output.wav
```

### `generator am <amplitude> <carrier_hz> <modulation_hz> <depth> <duration_ms>`

Создает сигнал с амплитудной модуляцией и заменяет им текущий сигнал.

- `amplitude` должен быть в диапазоне `[0, 1]`.
- `carrier_hz` и `modulation_hz` должны быть неотрицательными.
- `depth` должен быть в диапазоне `[0, 1]`.
- `duration_ms` должен быть неотрицательным.

Пример:

```bash
./build/sound_processor -f generator am 0.5 440 4 0.8 1000 -o Output.wav
```

### `generator fm <amplitude> <carrier_hz> <modulation_hz> <deviation_hz> <duration_ms>`

Создает сигнал с частотной модуляцией и заменяет им текущий сигнал.

- `amplitude` должен быть в диапазоне `[0, 1]`.
- `carrier_hz` должен быть неотрицательным.
- `modulation_hz` должен быть положительным.
- `deviation_hz` должен быть неотрицательным.
- `duration_ms` должен быть неотрицательным.

Пример:

```bash
./build/sound_processor -f generator fm 0.5 440 5 100 1000 -o Output.wav
```

## Публичный C++ API

### `Application`

Класс верхнего уровня для настройки и запуска приложения.

- `void configure()` — регистрирует доступные фильтры.
- `int start(int argc, char* argv[])` — разбирает аргументы, читает входной файл,
  собирает pipeline, применяет фильтры и записывает результат.

### `ArgsParser`

Разбирает аргументы командной строки.

- `Result parse(int argc, char* argv[])` — возвращает результат разбора.
- `getInFileName()` — возвращает входной путь, если он задан.
- `getOutFileName()` — возвращает выходной путь, если он задан.
- `getFilters()` — возвращает список `FilterDescriptor`.

Связанные типы:

- `FilterDescriptor` — имя фильтра и список строковых параметров.
- `Result::ok` — аргументы корректны.
- `Result::noArgs` — аргументы не переданы, нужно показать справку.
- `Result::badArgs` — ошибка синтаксиса аргументов.

### `Waveform`

Контейнер сэмплов и параметры поддерживаемого WAV-формата.

Константы:

- `SAMPLE_RATE = 44100`.
- `SAMPLE_SIZE = 16`.
- `AUDIO_FORMAT = 1` — PCM.
- `CHANNEL_COUNT = 1` — моно.

Функции:

- `durationSeconds()` — длительность сигнала в секундах.
- `samplesCount()` — количество сэмплов.
- `secondsToSamples(seconds)` — перевод секунд в сэмплы.
- `millisecondsToSamples(milliseconds)` — перевод миллисекунд в сэмплы.
- `samplesToSeconds(samples)` — перевод сэмплов в секунды.
- `samplesToMilliseconds(samples)` — перевод сэмплов в миллисекунды.
- `appendSample(sample)` — добавить сэмпл.
- `getSample(index)` — получить сэмпл по индексу.
- `setSample(index, sample)` — изменить сэмпл по индексу.
- `replaceSamples(samples)` — заменить все сэмплы.
- `clear()` — очистить сигнал.
- `getSamples()` — получить ссылку на вектор сэмплов.

### `Pipeline`

Хранит и выполняет последовательность фильтров.

- `addFilter(filter)` — добавляет фильтр в конец pipeline.
- `apply(sound)` — применяет все фильтры к `Waveform`.
- `getFilterCount()` — возвращает количество фильтров.

### `PipelineConverter`

Преобразует `FilterDescriptor` из CLI в готовый `Pipeline`.

- `addFilterProducer(filterName, producer)` — регистрирует фабрику фильтра.
- `createPipeline(filters)` — создает pipeline по списку дескрипторов.

### `WavReader`

Читает WAV-файл в `Waveform`.

- `Waveform read(const std::string& path)` — читает файл. Путь должен иметь
  расширение `.wav`.

Поддерживаются только моно PCM WAV-файлы: 16-bit, 44100 Hz.

### `WavWriter`

Записывает `Waveform` в WAV-файл.

- `void write(const std::string& path, const Waveform& waveform)` — записывает
  файл. Путь должен иметь расширение `.wav`.

### `IFilter`

Базовый интерфейс фильтра.

- `virtual void apply(Waveform& sound) = 0` — применяет фильтр к сигналу.

### Фильтры

- `Ampl(double factor)` — усиление/ослабление сигнала.
- `Normalize(double peak = 1.0)` — нормализация амплитуды.
- `Silence(const std::string& unit, double start, double end)` — вставка тишины.
- `TimeStretch(double factor)` — изменение длительности.
- `Lowpass(int windowSize)` — сглаживание скользящим средним.
- `SineGenerator(double frequencyHz, double durationMs)` — генерация синуса.
- `AmGenerator(double amplitude, double carrierHz, double modulationHz, double depth, double durationMs)` — генерация AM-сигнала.
- `FmGenerator(double amplitude, double carrierHz, double modulationHz, double deviationHz, double durationMs)` — генерация FM-сигнала.

### `FilterProducers`

Фабрики, которые создают фильтры из `FilterDescriptor`:

- `createAmplFilter(descriptor)`.
- `createNormalizeFilter(descriptor)`.
- `createSilenceFilter(descriptor)`.
- `createTimeStretchFilter(descriptor)`.
- `createLowpassFilter(descriptor)`.
- `createGeneratorFilter(descriptor)`.

## Ошибки

Программа выводит сообщение об ошибке в `stderr` и завершает работу с ненулевым
кодом, если:

- аргументы командной строки имеют неверный синтаксис;
- входной или выходной путь не имеет расширения `.wav`;
- WAV-файл не соответствует поддерживаемому формату;
- указан неизвестный фильтр или генератор;
- параметры фильтра имеют неверный тип, количество или диапазон.
