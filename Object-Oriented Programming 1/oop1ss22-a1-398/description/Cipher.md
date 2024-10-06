# Cipher

Verschlüsselungsalgorithmen können verwendet werden, um geheime Nachrichten über einen öffentlichen Kanal zu versenden. Es gibt mehrere Arten von Verschlüsselungsverfahren, welche im Laufe des Studiums noch genauer beleuchtet werden. Wir werden in diesem Beispiel einfache symmetrische Verschlüsselungsalgorithmen verwenden, um Nachrichten zwischen Benutzer:innen austauschen zu können. Für symmetrische Verschlüsselungsalgorithmen ist ein Schlüssel (_Key_) notwendig. Abhängig vom diesem Key werden die einzelnen Zeichen verschlüsselt. Eine Einführung in Verschlüsselungen gibt beispielsweise dieses kurze [Youtube Video](https://www.youtube.com/watch?v=DcX77qgbBZw).

In diesem Assignment sind zwei Verschlüsselungsalgorithmen zu implementieren, welche nachfolgend genauer beschrieben werden. Hierfür ist eine Basisklasse für Verschlüsselungen zu implementieren, von welcher eine Klasse pro konkretem Verschlüsselungsalgorithmus abgeleitet werden soll.

---

## `CipherType` & `Key`

Folgender Datentyp soll die möglichen Verschlüsselungsalgorithmen darstellen:

```
enum class CipherType { INVALID, NONE_CIPHER, ASCII_CIPHER, CAESAR_CIPHER };
```

Die Werte `CipherType::NONE_CIPHER`, `CipherType::ASCII_CIPHER` und `CipherType::CAESAR_CIPHER` stehen dabei jeweils für einen Algorithmus. Eine Beschreibung dieser Verschlüsselungsalgorithmen findest du unten. Der Wert `CipherType::INVALID` repräsentiert lediglich einen ungültigen Wert.

Außerdem soll ein neuer Datentyp `Key` mittels `typedef` erstellt werden. Ein `Key` soll durch eine 64 Bit umfassende Ganzzahl (genauer `uint64_t`) repräsentiert werden.

```
typedef uint64_t Key;
```

---

## Klasse `Cipher`

> Cipher.cpp und Cipher.hpp

Die Klasse `Cipher` stellt die *abstrakte* Basisklasse aller Verschlüsselungsalgorithmen dar.

Für alle Attribute und Methoden soll selbständig entschieden werden, ob diese `private`, `protected` oder `public` sein sollen. Inbesondere soll dabei der `protected` Specifier immer dann eingesetzt werden, wenn ein Attribut oder eine Methode von einer abgeleiteten Klasse verwendet wird, aber nicht von außen zugänglich sein soll.

### Attribute

- `key_`
  - Jeder Verschlüsselungsalgorithmus hat einen Schlüssel (_Key_), mit welchem die Zeichen einer Nachricht verschlüsselt werden.
  - Der Zugriff auf dieses Attribut soll von abgeleiteten Klassen aus möglich sein.
  - Datentyp: `Key`

### Konstruktoren

- `Cipher(Key key)`
  - setzt das Attribut `key_` auf `key`
- Der Copy-Konstruktor soll gelöscht werden.
- Als Destruktor soll der default-Destruktor verwendet werden.

### Methoden

- `Key getKey()`
  - Getter für `key_`
- `std::string getCipherTypeString() const`
  - eine *rein virtuelle* Methode (pure virtual function)
  - gibt den jeweiligen Verschlüsselungsalgorithmus als `std::string` zurück
- `std::string preparePlainText(const std::string& plain_text) const`
  - wandelt alle Buchstaben in Großbuchstaben um und löscht alle Leerzeichen aus dem Klartext
  - Rückgabe: der aufbereitete Klartext
- `std::string encrypt(const std::string& plain_text)`
  - eine *rein virtuelle* Methode (pure virtual function)
  - verschlüsselt den `cipher_text` mit dem jeweiligen Verschlüsselungsalgorithmus
  - Rückgabe: der Ciphertext
- `std::string decrypt(const std::string& cipher_text)`
  - eine *rein virtuelle* Methode (pure virtual function)
  - enschlüsselt den `cipher_text` mit dem jeweiligen Verschlüsselungsalgorithmus
  - Rückgabe: der Klartext
- `static CipherType getCipherType(const std::string& type)`
  - gibt `CipherType::NONE_CIPHER` zurück, falls `type` aus den Großbuchstaben NONE besteht
  - gibt `CipherType::ASCII_CIPHER` zurück, falls `type` aus den Großbuchstaben ASCII besteht
  - gibt `CipherType::CAESAR_CIPHER` zurück, falls `type` aus den Großbuchstaben CAESAR besteht
  - Trifft keiner der drei obigen Fälle ein, soll `CipherType::INVALID` zurückgegeben werden.

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.

---





## Klasse `NoneCipher`

> NoneCipher.cpp und NoneCipher.hpp

Die Klasse `NoneCipher` stellt eine nicht verschlüsselte Nachricht dar und ist von der Basisklasse `Cipher` abgeleitet. Diese Klasse ist hauptsächlich dazu gedacht das selbstständige Testen zu vereinfachen, da hier keine Verschlüsselung der Nachrichten vorgenommen wird. 

### Konstruktoren

- `NoneCipher(Key key)`
  - übergibt `key` an den Konstruktor der Basisklasse
  - (Der Key wird zwar nicht verwendet, muss aber trotzdem abgespeichert werden damit ein gültiges Konfigurationsfile erstellt werden kann)
- Der Copy-Konstruktor soll gelöscht werden.
- Als Destruktor soll der default-Destruktor verwendet werden.

### Methoden

- `std::string getCipherTypeString() const override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - gibt `"NONE"` zurück
- `std::string encrypt(const std::string& plain_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - der übergebene `plain_text` soll lediglich mit Hilfe der Methode `preparePlainText` aus der Basisklasse aufbereitet werden
  - Rückgabe: der aufbereitete Plaintext
- `std::string decrypt(const std::string& cipher_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - da in diesem Fall keine Verschlüsselung vorgenommen wurde kann der Text unverändert wieder zurückgegeben werden
  - Rückgabe: der Klartext

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.



------





## Ascii-Verschlüsselung

### Funktionsweise

Um einen Klartext verschlüsseln zu können, muss dieser zuerst mit `preparePlainText` aufbereitet werden.

Anschließend wird jeder Buchstabe aus dem aufbereiteten Klartext entsprechend der ASCII-Tabelle in einen Integer umgewandelt. Von diesem Integer wird nun `key_` modulo 10 abgezogen und der erhaltene Wert entspricht dem verschlüsselten Buchstaben.

Das Entschüsseln erfolgt analog, wobei nun zu jeder Zahl `key_` modulo 10 addiert werden muss.

### Beispiel

```
Key: 1511BEE9394FFFFA (hexadezimal) = 1518204458298769402 (dezimal)
Key modulo 10: 1518204458298769402 % 10 = 2
Klartext: "OOP"
Verschlüsseln:
  1. O = 79 => 79 - 2 = 77
  2. O = 79 => 79 - 2 = 77
  3. P = 80 => 80 - 2 = 78
Ciphertext: "77 77 78"
```
Man beachte die Leerzeichen im Ciphertext zwischen den einzelnen Zahlen. Weiters ist aufgrund der Konstruktion jede Zahl genau zweistellig.

```
Key: wie oben
Ciphertext: "77 77 78"
Entschlüsseln:
  1. 77 => 77 + 2 = 79 = O
  2. 77 => 77 + 2 = 79 = O
  3. 78 => 78 + 2 = 80 = P
Klartext: "OOP"
```

## Klasse `AsciiCipher`

> AsciiCipher.cpp und AsciiCipher.hpp

Die Klasse `AsciiCipher` stellt den ASCII-Cipher als konkreten Verschlüsselungsalgorithmen dar und ist von der Basisklasse `Cipher` abgeleitet.

### Konstruktoren

- `AsciiCipher(Key key)`
  - übergibt `key` an den Konstruktor der Basisklasse
- Der Copy-Konstruktor soll gelöscht werden.
- Als Destruktor soll der default-Destruktor verwendet werden.

### Methoden

- `std::string getCipherTypeString() const override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - gibt "ASCII" zurück
- `std::string encrypt(const std::string& plain_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - der übergebene `plain_text` soll zunächst mithilfe der Methode `preparePlainText` aus der Basisklasse aufbereitet werden
  - dann soll der aufbereitete `plain_text` laut ASCII-Verschlüsselung verschlüsselt werden
  - Rückgabe: der Ciphertext
- `std::string decrypt(const std::string& cipher_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - der verschlüsselte Text soll laut ASCII-Verschlüsselung entschlüsselt werden
  - *Hinweis:* Eine Methode aus der vorgegebenen Klasse `Utils` könnte hilfreich sein!
  - Rückgabe: der Klartext

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.

---





## Caesar-Verschlüsselung

### Funktionsweise

Um einen Klartext verschlüsseln zu können, muss dieser zuerst mit `preparePlainText` aufbereitet werden.

Bei der [Caesar-Verschlüsselung](https://de.wikipedia.org/wiki/Caesar-Verschl%C3%BCsselung) werden die einzelnen Buchstaben des Klartexts durch Additions eines sogenannten _Rotationswerts_ "verschoben" und dadurch verschlüsselt. Bei der einfachen Caesar-Verschlüsselung ist der Rotationswert für jeden Buchstaben des zu verschlüsselnden Textes gleich. Die hier zu implementierende Caesar-Variante ist etwas anspruchsvoller gestaltet, da jeder Buchstabe um einen anderen Wert verschoben werden kann.

Als Rotationswert wird bei dieser Version des Caesar-Ciphers immer das nächsthöhere Byte von `key_` verwendet und modulo 26 gerechnet. Angenommen `0102030405060708` ist der `key_`, dann wird im ersten Schritt `01 % 26`, im zweiten Schritt `02 % 26`, u. s. w. als Rotationswert verwendet. Nachdem das letzte Byte verwendet wurde, im Beispiel `08`, soll im nächsten Schritt (also für den nächsten Buchstaben) wieder mit dem höchstwertigen Byte – im Beispiel `01` – fortgefahren werden.

Der Rotationswert wird also für jeden Buchstaben im Klartext neu bestimmt und zum Buchstaben addiert. (Man spricht von _Rotation nach rechts_.). Sollte der dabei erhaltene Wert größer als `Z` sein, soll wieder am Beginn des Alphabets angefangen werden (siehe Beispiel unten).

Das Entschlüsseln verläuft analog. Der Unterschied zum Verschlüsseln besteht nur darin, dass alle Buchstaben des Ciphertexts nach links statt nach rechts rotiert werden. Es wird also der Rotationswert von den Buchstaben abgezogen. Hier gilt analog: Sollte der dabei erhaltene Wert kleiner als `A` sein, soll am Ende des Alphabets angefangen werden.

### Beispiel

```
Key: 0102030405060708
Plaintext: "EASYPEASY"
Verschlüsseln:
  1. E + 01 % 26 = E + 1 = F
  2. A + 02 % 26 = A + 2 = C
  3. S + 03 % 26 = S + 3 = V
  4. Y + 04 % 26 = Y + 4 = C (Y + 1 = Z, Y + 2 = A, ...)
  5. P + 05 % 26 = P + 5 = U
  6. E + 06 % 26 = E + 6 = K
  7. A + 07 % 26 = A + 7 = H
  8. S + 08 % 26 = S + 8 = A (S + 7 = Z, S + 8 = A)
  9. Y + 01 % 26 = Y + 1 = Z
Ciphertext: "FCVCUKHAZ"
```



## Klasse `CaesarCipher`

> CaesarCipher.cpp und CaesarCipher.hpp

Die Klasse `CaesarCipher` stellt den CaesarCipher als konkreten Verschlüsselungsalgorithmen dar und ist abgeleitet von der Basisklasse `Cipher`.

### Konstruktoren

- `CaesarCipher(Key key)`
  - übergibt `key` an den Konstruktor der Basisklasse
- Der Copy-Konstruktor soll geschlöscht werden.
- Als Destruktor soll der default-Destruktor verwendet werden.

### Methoden

- `std::string getCipherTypeString() const override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - gibt "CAESAR" zurück
- `std::string encrypt(const std::string& plain_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - Der übergebene `plain_text` soll zunächst mithilfe der Methode `preparePlainText` aus der Basisklasse aufbereitet werden.
  - Dann soll der aufbereitete `plain_text` gemäß Caesar-Verschlüsselung verschlüsselt werden.
  - Rückgabe: der Ciphertext
- `std::string decrypt(const std::string& cipher_text) override`
  - Implementierung der rein virtuellen Methode aus der Basisklasse `Cipher`
  - Der verschlüsselte Text soll gemäß Caesar-Verschlüsselung entschlüsselt werden.
  - Rückgabe: der Klartext

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.
