# Podstawowy line follower do zajęć edukacyjnych 

## Opis projektu

Projekt ten jest przykładem prostego line followera, który może być wykorzystany do zajęć edukacyjnych. Program jest napisany w języku C z wykorzystaniem platformy Arduino.

## Tabela zawartości

- [Podstawowy line follower do zajęć edukacyjnych](#podstawowy-line-follower-do-zajęć-edukacyjnych)
  - [Opis projektu](#opis-projektu)
  - [Tabela zawartości](#tabela-zawartości)
  - [Uruchamianie programu](#uruchamianie-programu)
    - [Krok 1: Sprawdzenie środowiska uruchomieniowego Arduino](#krok-1-sprawdzenie-środowiska-uruchomieniowego-arduino)
    - [Krok 2: Sprawdzenie podłączenia pinów](#krok-2-sprawdzenie-podłączenia-pinów)
    - [Krok 3: Załadowanie programu na płytkę ESP 32 WROOM DA](#krok-3-załadowanie-programu-na-płytkę-esp-32-wroom-da)
    - [Krok 4: Ulepszanie programu i eksperymentowanie](#krok-4-ulepszanie-programu-i-eksperymentowanie)
      - [Wykrywanie linii](#wykrywanie-linii)
      - [Sterowanie silnikami](#sterowanie-silnikami)
        - [Stan robota](#stan-robota)
    - [Eksperymenty](#eksperymenty)
      - [Podstawowe zmiany](#podstawowe-zmiany)
      - [Zaawansowane zmiany w algorytmie](#zaawansowane-zmiany-w-algorytmie)
        - [Poprawa reakcji przy wyjściach z zakrętu.](#poprawa-reakcji-przy-wyjściach-z-zakrętu)
        - [Korekta siły skrętu](#korekta-siły-skrętu)
        - [Kontrola jazdy za pomocą jednego czujnika](#kontrola-jazdy-za-pomocą-jednego-czujnika)
  - [Dodatkowe informacje o platformie](#dodatkowe-informacje-o-platformie)


## Uruchamianie programu

### Krok 1: Sprawdzenie środowiska uruchomieniowego Arduino

Upewnij się, że masz zainstalowane środowisko uruchomieniowe Arduino. 
Jeśli nie, pobierz je ze strony [Arduino](https://www.arduino.cc/en/software).


### Krok 2: Sprawdzenie podłączenia pinów

Sprawdź, czy wszystkie piny są odpowiednio podłączone. Poniżej znajdziesz odpowiedni pinout:

| Nazwa pinu | Funkcja       | Nazwa w kodzie        | Wartość w kodzie |
| ---------- | ------------- | --------------------- | ---------------- |
| D15        | Czujnik lewy  | IR_SENSOR_INPUT_LEFT  | 15               |
| D14        | Czujnik prawy | IR_SENSOR_INPUT_RIGHT | 14               |
| D12        | Lewy silnik   | LEFT_MOTOR_PIN        | 12               |
| D13        | Prawy silnik  | RIGHT_MOTOR_PIN       | 13               |


### Krok 3: Załadowanie programu na płytkę ESP 32 WROOM DA

| :zap:        Uwaga!                                                                                                                                        |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Przed załadowaniem programu na płytkę, wyjmij jedną baterię AA z obwodu zasilania silników. W przeciwnym razie, program nie zostanie załadowany na płytkę. |

W Arduino menu upewnij się, że:
- Wybrana jest odpowiednia płytka. W górnej zakładce wybierz z listy płytek odpowiednią nazwę -- w naszym przypadku jest to `ESP 32 WROOM DA`. 
- Kolejnym krokiem jest upewnienie się, że wybrany jest odpowiedni port szeregowy. W menu `Tools` wybierz odpowiedni port szeregowy, na którym jest podłączona płytka (`Tools` --> `Port`).
- Skopiuj kod z pliku `follower.ino` do edytoru programu Arduino.
- Załaduj program na płytkę ESP 32 WROOM DA. W tym celu kliknij przycisk `Upload` w górnym lewym rogu okna programu Arduino.
- Jeżeli wszystko przebiegło pomyślnie, program powinien zostać załadowany na płytkę ESP 32 WROOM DA i nie powinno być żadnych błędów w zakładce `Output`.
- Po załadowaniu programu, uruchom płytkę ESP 32 WROOM DA i sprawdź, czy działa poprawnie.
- Możesz sprawdzić, czy program działa poprawnie, przykładając do czujników linii jakiś kontrastowy materiał. 
  W przypadku poprawnego działania silniki powinny zacząć się obracać. Dodatkowo, jeżeli czujniki wykryją linię, to silniki powinny zacząć się obracać w kierunku wykrytej linii. Jako pomoc, możesz skorzystać z monitoru portu szeregowego, algorytm powinien wyświetlać informacje o aktualnym trybie jazdy (`Tools` --> `Serial Monitor`).

### Krok 4: Ulepszanie programu i eksperymentowanie

Podstawowym algorytmem, który jest zaimplementowany w programie, jest algorytm wykrywania linii i sterowania silnikami.
Znajduje się on w funkcji `drive` w pliku `follower.ino`.

#### Wykrywanie linii
Podstawowy algorytm korzysta z dwóch czujników IR, które są umieszczone z przodu robota.
Sensory IR działają na zasadzie odbicia światła od powierzchni -- fotoemiter emituje światło w zakresie podczerwonym (infrared -- IR), a fotoodbiornik mierzy ilość światła odbitego od powierzchni. Czarne powierzchnie odbijają mniej światła, a białe powierzchnie odbijają więcej światła, dzięki czemu jesteśmy w stanie rozróżnić czarną linię na jasnym tle.
Jeżeli czujnik wykryje linię, to zwróci wartość 1, w przeciwnym razie zwróci wartość 0, jeśli jest czujnikiem cyfrowym. W przypadku czujników analogowych, wartość zwracana jest w zakresie od 0 do 4096. Czarna linia zwróci w przypadku takich czujników wartość bliską 4096. 

Próg wykrycia linii jest kontrolowany przez zmienną `LINE_THRESHOLD` w kodzie. Jeżeli masz czujnik analogowy, to wartość `LINE_THRESHOLD` powinna być ustawiona na wartość, która jest bliska wartości zwracanej przez czujnik, gdy wykryje linię. W przypadku czujników cyfrowych, wartość `LINE_THRESHOLD` powinna być ustawiona 
w przedziale [0, 4096], a jeżeli czujnik jest cyfrowy, to wartość `LINE_THRESHOLD` powinna być ustawiona w przedziale [0, 1].

#### Sterowanie silnikami
Sterowanie silnikami jest realizowane poprzez zmianę stanu pinów, do których są podłączone silniki. W zależności od konfiguracji, silniki mogą być sterowane za pomocą sygnałów PWM lub sygnałów cyfrowych. 
W przypadku sygnałów PWM, wartość sygnału PWM jest ustawiana na wartość z zakresu [0, 255], gdzie 255 oznacza zatrzymanie silnika, 
a 0 oznacza maksymalną prędkość. Więcej informacji o PWM w Ardunio znajdziesz [tutaj](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/) oraz [tutaj](https://docs.arduino.cc/learn/microcontrollers/analog-output/).


| :zap:        Uwaga!                                                                                                                                                                                                                          |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Kazdy silnik jest sterowany oddzielnym PWM. Dostępne są 2 kanały, ale korzystamy tylko, z tego który pozwala na obrót wprzód. Drugi kanał dla kadego silnika odpowiada za obrót w tył. Nie korzystamy z niego podczas naszych eskperymentów. |


Logika sterowania silnikami jest prosta. W zależności od wykrytej linii, silniki obracają się w odpowiednim kierunku.
W momencie, gdy oba czujniki wykryją linię, to oba silniki dostają wartość PWM równą `HIGH_SPEED`, czyli maksymalnej prędkości. Kiedy lewy sensor wykrywa linię, a prawy nie,
wtedy nalezy skręcić w lewo, czyli lewy silnik dostaje wartość `LOW_SPEED`, a prawy `CRUISE_SPEED`. W przeciwnym przypadku, skręcamy w prawo, czyli lewy silnik dostaje wartość `CRUISE_SPEED`, a prawy `LOW_SPEED`.
`CRUISE_SPEED` jest nieco niższa niż `HIGH_SPEED`, ale nieco wyższa niż `LOW_SPEED`, by robot mógł łagodniej skręcać.

##### Stan robota

Dla wygody oznaczyliśmy możliwe stany robota i silników w `enum` `state. Są to:
- `FORWARD` -- robot jedzie prosto, oba czujniki wykrywają, linię `HIGH_SPEED` na obu silnikach
- `LEFT` -- robot skręca w lewo, lewy silnik dostaje `LOW_SPEED`, a prawy `CRUISE_SPEED`
- `RIGHT` -- robot skręca w prawo, lewy silnik dostaje `CRUISE_SPEED`, a prawy `LOW_SPEED`
- `STOP` -- robot zatrzymuje się. Zatrzymanie robota jest realizowane poprzez ustawienie wartości PWM na obu silnikach na 255.

| :zap: Uwaga                                                                                                                                                                                                                                                                                                                                        |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Silniki mają strefę "martwą". Co to oznacza? Mniej więcej tyle, że każdy silnik wymaga minimalnej wartości PWM, aby ruszyć. Pamiętaj, mamy 2 niezależne servo silniki w każdym robocie, więc każdy z nich może mieć inną strefę martwą! Przykładowo, lewy silnik może potrzebować minimalnej wartości na pinie, aby ruszyć równej 129, a drugi 120. |

### Eksperymenty

#### Podstawowe zmiany 

Spróbuj zmienić wartości i zobacz, jak zmienia się zachowanie robota:

- `LINE_THRESHOLD` 
- `HIGH_SPEED`
- `LOW_SPEED`
- `CRUISE_SPEED`


Mamy jeszcze jedną zmienną, która może być przydatna. Jest to `bufLen` i służy do kontroli powrotu na tor. Zamiast zatrzymywać się od razu w momencie, gdy robot traci linię na obu czujnikach, próbujemy powtórzyć ostatnie polecenie [stan](#stan-robota) `bufLen` razy, próbując wrócić na tor. Jeżeli się to nie uda w `bufLen` próbach, to zatrzymujemy robota.

**Przykład:**
Robot skręca w prawo, ale nagle traci linię, bo nie wyrobił się na zakręcie. Zamiast od razu zatrzymać robota, próbujemy skręcić zadać silnikom stan skrętu w prawo `bufLen` razy, zanim ostatecznie zatrzymamy robota.


#### Zaawansowane zmiany w algorytmie

Jeżeli masz trochę więcej zacięcia, to możesz spróbować dostosować algorytm, by dalej upłynnić jazdę. Poniżej zamieszczamy kilka pomysłów, które możesz wykorzystać:

##### Poprawa reakcji przy wyjściach z zakrętu. 

Załóżmy, że robot podjeżdża do [zakrętu w prawo, stan RIGHT](#stan-robota). W podstawowym przypadku naszego algorytmu, który omówiliśmy wcześniej, robot ustawi `LOW_SPEED` na prawym silniku, a `CRUISE_SPEED` na lewym. 
Tor jazdy zostanie skorygowany, ale jest możliwe, że po jednym takim sygnale już oba czujniki wykryją linię, mimo że zakręt niekoniecznie się skończył. Oznacza to, że w takim przypadku oba silniki dostaną sygnał `HIGH_SPEED`, co może skutkować wypadnięciem z toru przy zbyt dużej prędkości, jeśli nadal nie wyszliśmy z zakrętu. 

Możemy poprawić to w następujący sposób, poprzez wprowadzenie bufora kontroli wychodzenia z zakrętu. [Gdy oba czujniki wykryją linię to](#stan-robota), zamiast od razu ustawić `HIGH_SPEED`, możemy próbujemy najpierw niższą wartość `TURN_SPEED` na obu silnikach. Dopóki nie wykryjemy `n` razy z rzędu stanu `FORWARD`, będziemy używali mniejszej siły napędowej. Dopiero po wykryciu `n` razu linii przez oba czujniki pozwalamy robotowi jechać z pełną prędkością do przodu, `HIGH_SPEED`.

Wyzwaniem będzie nie tylko napisanie takie algorytmu, co nie jest trudne, ale dostrojenie wartości `n` i `TURN_SPEED`, tak, aby robot jak najefektywniej wychodził z zakrętów.

##### Korekta siły skrętu

Podobnie jak w przypadku wyżej, możemy próbować regulować jak mocno robot będzie skręcał. Przykładowo, za pierwszym razem, kiedy któryś z czujników utraci linię, zamiast wartości `CRUISE_SPEED` i `LOW_SPEED` możemy ustawić mniejsze wartości, e.g. 0.2*`CRUISE_SPEED` oraz `LOW_SPEED`. Możemy stopniowo zwiększać wartości, aż do osiągnięcia wartości `CRUISE_SPEED` i `LOW_SPEED`, i wtedy zacząć skręcać z pełną siłą. Potem możemy stosować to samo co w punkcie 1, czyli bufor wychodzenia z zakrętu. Mnożnik 0.2 jest podany obrazowo, musisz go dostosować pamiętając o [strefie martwej silników](#sterowanie-silnikami). Tak jak poprzednio, wyzwanie polega na dostrojeniu wartości mnożnika, minimalnej prędkości skrętu na obu silnikach oraz wartości `n` w buforze skręcania.


##### Kontrola jazdy za pomocą jednego czujnika

W naszym modelu korzystamy z obu silników, ale w kodzie zamieściliśmy przykład, pokazujący w jaki sposób można korzystać z tylko jednego czujnika, w funkcji `drive_single`. Spróbuj dostroić algorytm, który korzysta tylko z jednego czujnika -- nie musisz modyfikować mechaniki, po prostu w kodzie możesz przekazywać do funkcji `drive_single` tylko wartości z jednego czujnika (albo prawego albo lewego). Pamiętaj, że wtedy musisz dostosować wartości `HIGH_SPEED`, `LOW_SPEED` i `CRUISE_SPEED` do nowych warunków.

## Dodatkowe informacje o platformie

Projekt został napisany z wykorzystaniem platformy ESP 32 WROOM. Poniżej znajdziesz informacje o platformie:

- [ESP 32 WROOM](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
- [Sensor IR, zasada działania](https://www.electronicsforu.com/technology-trends/learn-electronics/ir-led-infrared-sensor-basics)
- [Servo silniki](https://www.sparkfun.com/servos)

