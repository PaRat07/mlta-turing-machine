Задание начального состояния:
- T - мн-во теримналов
- N - мн-во нетерминалов
- Q - мн-во состояний (opt)
- B - мн-во переходов

Задается конфигом yaml\
Пример:
```yaml
alphabet:
    terms: [1, 0, "#"]
    non-terms: []
    blank: "#"

states: [S, F, Fwd, Ret]
initial-state: S
halt-state: F

tape-init: tape.txt

transitions:
	S:
		"%": [_, Fwd, _]
	Fwd:
		1: [0, _, r]
		0: [0, _, r]
		"#": [_, Ret, l]
	Ret:
		"#": [_, F, r]
		"*": [_, _, l]
```

Выбор перехода происходит сверху вниз по "первый подходящий". Вместо символа в качестве условия может стоять "*", что значит "любой символ".

### Сборка и запуск:
```shell
docker build . -t turing-machine
```

```shell
docker run -v "$(pwd)/example-input:/example-input" --rm turing-machine:latest /example-input/<ИМЯ ВХОДНЫХ ДАННЫХ>.yaml
```
