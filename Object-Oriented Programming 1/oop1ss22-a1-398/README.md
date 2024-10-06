# Assignment 1 - Messaging-Dienst

In Assignment 1 (A1) der Konstruktionsübung (KU) soll ein Programm geschrieben werden, mit dem Nachrichten – ähnlich wie bei einem Messaging-Dienst – in verschlüsselter Form zwischen Benutzer:innen ausgetauscht werden können. Als Verschlüsselungsalgorithmen sollen zwei einfache klassische Verfahren (wie beipielsweise eine Version des Caesar-Ciphers) implementiert werden.

---

## Lernziele

- Grundlegendes über Klassen
  - Attribute, Methoden
  - Konstruktor, Copy-Konstruktor, Destruktor
- Vererbung
  - virtuelle Methoden
  - abstrakte Klassen
- Strings & Streams (werden am 31.3. in der VO vorgestellt)
  - `std::string`
  - Files
- Datenstrukturen aus der Standard Template Library
  - bspw. `std::vector` oder `std::map`

---

## Spezifikation

- offizielle Ausgabe: 24.3.2022 nach dem VO-Stream
- Abzugeben bis (Soft-Deadline): 19.4.2022 um 23:59 (in Österreich gültige Zeit)
- Nachfrist für eine Ausbesserung bis (Hard-Deadline): 23.4.2022 um 23:59 (in Österreich gültige Zeit)
- Das Framework bestehend aus `main.cpp`, `IO.hpp`, `IO.cpp` sowie `Utils.hpp` und `Utils.cpp` dürfen **NICHT** verändert werden.
- Es dürfen nur C++ Standard Libraries verwendet werden
- A1 ist **keine** Gruppenarbeit. Jede:r Student:in erhält vom Lehrveranstaltungs-Team ein eigenes Gitlab-Repository und das Assignment ist **eigenständig** zu
  bearbeiten. Bitte lies dir dazu unsere [Richtlinien zu Plagiaten](https://tc.tugraz.at/main/mod/page/view.php?id=256924#Plagiate) genau durch!
- Die Abgabe muss auf einen eigenen Git-Branch mit dem Namen `submission` erfolgen. Das heißt vor Ablauf der Nachfrist
  muss die eigene Lösung mittels `git push` in den `submission`-Branch des eigenen Gitlab-Repositorys für A1 gesendet worden sein.

---

## Bewertung

**Achtung: Für eine positive Beurteilung in der KU muss mindestens die Hälfte der öffentlichen Testcases bestanden werden.** 

Das Assignment wird, wie im [TeachCenter](https://tc.tugraz.at/main/mod/page/view.php?id=138898) beschrieben, bewertet, allerdings mit zwei Ausnahmen:

1. Beim A1 wird **nur die Funktionalität bewertet**, nicht aber der Coding Standard.
2. Zum A1 wird es **keine Abgabegespräche** geben.

Auch das Beurteilungsschema der Konstruktionsübung findet sich im [TeachCenter](https://tc.tugraz.at/main/mod/page/view.php?id=256924), insbesondere sei hier noch einmal auf die Regeln zu [Plagiaten](https://tc.tugraz.at/main/mod/page/view.php?id=256924#Plagiate) verwiesen.

---

## Beschreibung

### Überblick

In A1 ist ein Programm zu implementieren, in dem Nachrichten – ähnlich wie bei einem Messaging-Dienst – in verschlüsselter Form zwischen Benutzer:innen ausgetauscht werden.

Grundlage des Programms bildet eine Konfigurationsdatei, in der definiert ist, wer für diesen Messaging-Dienst registiert ist und diesen damit auch verwenden kann. Weiters wird in dieser Konfigurationsdatei festgehalten, mit welchen anderen Personen (Kontakten) ein:e Benutzer:in Nachrichten austauschen kann.

Damit nun ein:e Benutzer:in den Messaging-Dienst verwenden kann, muss sich diese:r zu Beginn des Programms einloggen. Falls dies erfolgreich war, ist es für die:den Benutzer:in möglich, Nachrichten an die eigenen Kontakte zu schicken bzw. auch bereits erhaltene Nachrichten zu lesen.

Wichtig ist hierbei, dass alle versendeten Nachrichten verschlüsselt in Textdateien abgespeichert werden, sodass es für andere Benutzer:innen nicht möglich ist, den Inhalt der Nachrichten einfach aus den Textdateien auszulesen. Im Umkehrschluss bedeutet das aber auch, dass erhaltene Nachrichten zuerst entschlüsselt werden müssen, damit diese gelesen werden können.

Anmerkung: Üblicherweise würde man Nachrichten zu einem Server schicken bzw. von diesem empfangen. Um dies in A1 zu vereinfachen, werden abgesendete Nachrichten lediglich in Textdateien abgespeichert.

---

### Klassen

#### Cipher

Um die Nachrichten verschlüsselt versenden zu können, benötigen wir einen Verschlüsselungsalgorithmus. In diesem Assignment werden Verschlüsselungsalgorithmen durch Klassen repräsentiert. Als Basisklasse für alle Verschlüsselungsalgorithmen soll eine Klasse namens `Cipher` dienen. In den von ihr abgeleiteten Klassen sollen die jeweiligen Verschlüsselungsalgorithmen implementiert werden. Eine detaillierte Beschreibung der Klassen ist unter [description/Cipher.md](description/Cipher.md) zu finden.

**Tipp:** Da mehrere konkrete Verschlüsselungsalgorithmen zu implementieren sind, raten wir, zunächst nur den einfachsten zu implementieren (None-Cipher) und sich dann um die in der Folge beschriebenen Klassen `User` und `Config` zu kümmern. Alle Verschlüsselungsalgorithmen können auch mithilfe der [Testoption](#Test) beim Programmstart einfach überprüft werden. 

#### User & Messages

Benutzer:innen des Messaging-Dienstes können verschlüsselte Nachrichten senden und empfangen und werden in diesem Programm durch die zu schreibende Klasse `User` repräsentiert. Eine detaillierte Beschreibung dieser Klasse ist unter [description/User_Messages.md](description/User_Messages.md) zu finden.

#### Config

Sind die oben genannten Klassen implementiert, kann mit dem Einlesen der Konfigurationsdatei begonnen werden. Eine detaillierte Beschreibung über den Aufbau der Konfigurationsdatei ist unter [description/Config.md](description/Config.md) zu finden. In dieser Datei wird auch beschrieben, wie die entsprechende Klasse namens `Config` aussehen soll.



---

### Programmstart

**ACHTUNG: Die Datei `main.cpp` darf NICHT veändert werden!**

In der Datei `main.cpp` ist der Programmstart bereits gegeben. Das Programm erhält beim Aufruf den Pfad zur Konfigurationsdatei als Parameter. Danach wird ein Objekt der Klasse `Config` (zu implementieren) erstellt, das File überprüft und versucht, die Konfigurationsdatei zu parsen (einlesen der Benutzer:innen und ihrer Kontakte).

War das Einlesen der Konfigurationsdatei erfolgreich, kann die:der Benutzer:in zwischen den  Optionen _Registrierung_, _Login_ und _Test_  wählen.

### Registrierung

> bereits implementiert in main.cpp

Wird bei der ersten Eingabe die Registrierung gewählt, dann wird die Funktion `userRegistration` aufgerufen und es müssen Benutzername und Passwort eingeben werden. Beim Benutzernamen wird überprüft, ob die:der Benutzer:in bereits in der Konfigurationsdatei vorhanden ist. Wenn nicht, dann soll die:der Benutzer:in mit dem Funktionsaufruf `config.registerUser(username, password)` registriert werden. Diese Funktion ist u. a. in der `Config`-Klasse zu implementieren.

Der Rückgabewert der Funktion `userRegistration` in der Datei `main.cpp` soll ein Pointer auf das `User`-Objekt sein, das die:den neu registrierte:n Benutzer:in repräsentiert.

#### Beispiel

```
Welcome to the OOP1 Messaging Service!
Would you like to [r]egister, [l]ogin or [t]est?
  > r
Please enter user name!
  > Max
Please enter your password!
  > easypassword


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



### Login

> bereits implementiert in main.cpp

Wird bei der ersten Eingabe der Login gewählt, dann wird die Funktion `userLogin` aufgerufen und es muss zunächst der Benutzername eingeben werden. Es wird in der Konfigurationsdatei überprüft, ob dieser Name vorhanden ist. Wenn dies der Fall ist, hat die:der Benutzer:in drei Mal die Möglichkeit, das richtige Passwort einzugeben. Hierfür wird die Funktion `config.loginUser(username, password)` aufgerufen, welche zu implementieren ist. Diese gibt einen Pointer auf das entsprechende `User`-Objekt zurück, wenn der Login erfolgreich war und `nullptr`, wenn das Passwort nicht mit jenem in der Konfigurationsdatei übereinstimmt. (`nullptr` ist das C++ Äquivalent zu `NULL`.)

Der Rückgabewert der Funktion `userLogin` in der Datei `main.cpp` soll der neu angelegte und eingeloggte User sein oder `nullptr` wenn der Login nicht erfolgreich war.

```
Welcome to the OOP1 Messaging Service!
Would you like to [r]egister, [l]ogin or [t]est?
  > l
Please enter user name!
  > David
Please enter your password!
  > easypassword


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



### Test

> bereits implementiert in main.cpp

Wird bei der ersten Eingabe Test ausgewählt, erfolgt kein Zugang zum Messaging Dienst. Diese Option dient lediglich dazu, die zu implementierenden Verschlüsselungsalgorithmen ohne die Notwendigkeit anderer Klassen testen zu können. 

#### Beispiel

```
Welcome to the OOP1 Messaging Service!
Would you like to [r]egister, [l]ogin or [t]est?
  > t
What cipher would you like to use?
  > caesar
Enter your message now:
  > hello world

Sending ...
INCKVLLWMM
Reading ...
Recipient: SYSTEM
Sender: SYSTEM
HELLOWORLD
```



### Menü

> bereits implementiert in main.cpp

War die Registrierung oder der Login erfolgreich, kommt die:der Benutzer:in ins Hauptmenü. Auch dieses ist bereits fertig implementiert. Eine detaillierte Beschreibung des Menüs ist unter [description/Menu.md](description/Menu.md) zu finden.



### Programmende

Dieser Teil ist bereits implementiert. Es ist jedoch noch zu implementieren, wann eine Konfigurationsdatei ungültig ist. 

| Rückgabewert | Beschreibung                                                 |
| ------------ | ------------------------------------------------------------ |
| 0            | Normales Programmende                                        |
| 1            | Speicher konnte nicht alloziert werden. Fehlermeldung: `Error: not enough memory!\n` |
| 2            | Falsche Anzahl an Programmargumenten. Fehlermeldung: `Usage: ./a1 <configfile>\n` |
| 3            | Konfigurationsdatei konnte nicht geöffnet werden. Fehlermeldung: `Error: could not open config file!\n` |
| 4            | Ungültige Konfigurationsdatei. Fehlermeldung: `Error: invalid config file!\n` |
| 5            | Login nicht erfolgreich nach drei fehlgeschlagenen Versuchen    |





## Hinweis

> Ein wichtiger Hinweis noch am Ende: Sämtliche kryptographische Verfahren, die in diesem Beispiel eingesetzt werden, sind keinesfalls sicher und sollten unter keinen Umständen in sicherheitskritischen Anwendungen verwendet werden. Bedingt durch die Konfigurationsdatei sind weiters sämtliche Schlüssel öffentlich einsehbar. Daher können auch alle mit Zugriff auf die Konfigurationsdatei jeden Schlüssel einsehen und damit jede Nachricht entschlüsseln. Bei Interesse über zeitgemäße kryptographische Verfahren, die aber wesentlich komplexer sind als die hier eingesetzten, dürfen wir auf die Lehrveranstaltung "Information Security" verweisen.
