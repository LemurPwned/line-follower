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
        - [Eksperymenty](#eksperymenty)
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

| :zap:        Uwaga!                                                                         |
| ------------------------------------------------------------------------------------------- |
| Przed załadowaniem programu na płytkę, wyjmij jedną baterię AA z obwodu zasilania silników. |
| W przeciwnym razie, program nie zostanie załadowany na płytkę.                              |

W Arduino menu upewnij się, że:
- wybrana jest odpowiednia płytka. W górnej zakładce wybierz z listy płytek odpowiednią nazwę -- w naszym przypadku jest to `ESP 32 WROOM DA`. 
- kolejnym krokiem jest upewnienie się, że wybrany jest odpowiedni port szeregowy. W menu `Tools` wybierz odpowiedni port szeregowy, na którym jest podłączona płytka.
- załaduj program na płytkę ESP 32 WROOM DA. W tym celu kliknij przycisk `Upload` w górnym lewym rogu okna programu Arduino.
- jezeli wszystko przebiegło pomyślnie, program powinien zostać załadowany na płytkę ESP 32 WROOM DA i nie powinno być żadnych błędów w zakładce `Output`.
- po załadowaniu programu, uruchom płytkę ESP 32 WROOM DA i sprawdź, czy działa poprawnie.
- możesz sprawdzić, czy program działa poprawnie, przykładając do czujników linii jakiś kontrastowy materiał. 
  W przypadku poprawnego działania, silniki powinny zacząć się obracać. Dodatkowo, jeżeli czujniki wykryją linię, to silniki powinny zacząć się obracać w kierunku wykrytej linii. 
  Jako pomoc,mozesz skorzystać z monitoru portu szeregowego, algorytm powinien wyświetlać informacje o aktualnym trybie jazdy.

### Krok 4: Ulepszanie programu i eksperymentowanie


#### Wykrywanie linii
Podstawowy algorytm korzysta z dwóch czujników linii, które są umieszczone z przodu robota.
Sensory IR działają na zasadzie odbicia światła od powierzchni -- jeżeli czujnik wykryje linię, to zwróci wartość `LOW`, w przeciwnym razie zwróci wartość `HIGH`, jeeli jest to czujnik cyfrowy. W przypadku czujników analogowych, wartość zwracana jest w zakresie od 0 do 4096. Czarna linia zwróci wartość bliską 4096. 

Próg wykrycia linii jest kontrolowany przez zmienną `LINE_THRESHOLD` w kodzie. Jezeli masz czujnik analogowy, to wartość `LINE_THRESHOLD` powinna być ustawiona na wartość, która jest bliska wartości zwracanej przez czujnik, gdy wykryje linię. W przypadku czujników cyfrowych, wartość `LINE_THRESHOLD` powinna być ustawiona 
w przedziale [0, 4096], a jezeli czujnik jest cyfrowy, to wartość `LINE_THRESHOLD` powinna być ustawiona w przedziale [0, 1].

#### Sterowanie silnikami
Sterowanie silnikami jest realizowane poprzez zmianę stanu pinów, do których są podłączone silniki. W zależności od konfiguracji, silniki mogą być sterowane za pomocą sygnałów PWM lub sygnałów cyfrowych. 
W przypadku sygnałów PWM, wartość sygnału PWM jest ustawiana na wartość z zakresu [0, 255], gdzie 255 oznacza zatrzymanie silnika, 
a 0 oznacza maksymalną prędkość. Więcej informacji o PWM w Ardunio znajdziesz [tutaj](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/) oraz [tutaj](https://docs.arduino.cc/learn/microcontrollers/analog-output/).


| :zap:        Uwaga!                                                                                                                                                                                                                          |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Kazdy silnik jest sterowany oddzielnym PWM. Dostępne są 2 kanały, ale korzystamy tylko, z tego który pozwala na obrót wprzód. Drugi kanał dla kadego silnika odpowiada za obrót w tył. Nie korzystamy z niego podczas naszych eskperymentów. |


Logika sterowania silnikami jest prosta. W zależności od wykrytej linii, silniki obracają się w odpowiednim kierunku.
Jeżeli oba czujniki wykryją linię, to oba silniki dostają wartość PWM równą `HIGH_SPEED`, czyli maksymalnej prędkości. Jezeli lewy sensor wykrywa linię, a prawy nie,
wtedy nalezy skręcić w lewo, czyli lewy silnik dostaje wartość `LOW_SPEED`, a prawy `CRUISE_SPEED`. W przeciwnym przypadku, skręcamy w prawo, czyli lewy silnik dostaje wartość `CRUISE_SPEED`, a prawy `LOW_SPEED`.
`CRUISE_SPEED` jest nieco niższa niż `HIGH_SPEED`, ale nieco wyższa niż `LOW_SPEED`, by robot mógł ładnie skręcać.

##### Eksperymenty

- Spróbuj zmienić wartości i zobacz, jak zmienia się zachowanie robota:
- `LINE_THRESHOLD` 
- `HIGH_SPEED`
- `LOW_SPEED`
- `CRUISE_SPEED`


## Dodatkowe informacje o platformie

Projekt został napisany z wykorzystaniem platformy ESP 32 WROOM. Poniżej znajdziesz informacje o platformie:

- [ESP 32 WROOM](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)

