# User & Messages

Im Folgenden wird die Klasse `User` beschrieben, die eine:n Benutzer:in im Programm repräsentiert. Weiters wird beschrieben, wie mit den Nachrichten umgegangen werden soll, die zwischen Benutzer:innen augetauscht werden.



## Klasse `User`

> User.cpp und User.hpp

Die Klasse `User` bildet eine:n Benutzer:in im Programm ab. In der [Konfigurationsdatei](Config.md) sind User der erste Eintrag jeder Zeile.

### Attribute

- `name_`
  - der Name der Person als `std::string`
- `password_`
  - das Passwort der Person als `std::string`
- `contacts_`
  - eine `std::map` mit einem `User*` als Key und einem `Cipher*` als Value und einer Sortierungsfunktion `bool (*)(User*, User*)`: `std::map<User*, Cipher*, bool(*)(User*, User*)> contacts_;`
  - Ein:e Benutzer:in kann eine beliebige Anzahl an Kontakten besitzen, welche in dieser Map abgespeichert werden sollen.
  - Jeder _Key_ der Map ist ein Pointer auf eine:n andere:n Benutzer:in, also auf einen Kontakt.
    - Achtung! Der Key einer Map hat nichts mit dem Key eines Verschlüsselungsalgorithmus zu tun.
  - Jeder _Value_ der Map ist ein Pointer auf den zu verwendenden `Cipher`. Dieser ist notwendig, um mit dem Kontakt verschlüsselt kommunizieren zu können.
  - Die Sortierungsfunktion dient dazu, dass die Kontaktliste alphabetisch sortiert ist. Es ist nicht notwendig, genau zu verstehen, was die Sortierungsfunktion `bool(*)(User*, User*)` macht. 

### Konstruktoren

- `User(const std::string& name, const std::string& password)`
  - setzt das Attribut `name_` auf `name`
  - setzt das Attribut `password_` auf `password`
  - setzt das Attribut `contacts_` auf `Utils::sortContactsAlphabetically` *(Tipp: Initialisierungsliste verwenden!)*
    - mit dieser Funktion sortiert die Map ihre Keys
- Der Copy-Konstruktor soll gelöscht werden.
- Es soll ein geeigneter Destruktor implementiert werden, der, falls nötig, allokierten Speicher wieder freigibt.

### Methoden

- `std::string getName() const`
  - Getter für `name_`
- `std::string getPassword() const`
  - Getter für `password_`
- `const auto& getContacts() const`
  - Getter für `contacts_`
  - zur Übersichtlichkeit kann einfach `auto&` anstatt des tatsächlichen Datentyps verwendet werden
- `bool verifyPassword(const std::string& password) const`
  - überprüft, ob das eingebenen Passwort (Parameter) mit dem gespeicherten Passwort übereinstimmt
  - gibt genau dann `true` zurück, wenn die Paswörter übereinstimmen
- `bool hasContact(const std::string& name) const`
  - überprüft, ob es eine:n Benutzer:in (Kontakt) mit dem Namen `name` in `contact_` gibt
  - gibt genau dann `true` zurück, wenn ein:e solche:r Benutzer:in vorhanden ist
- `void addContact(User* contact, CipherType type, Key key)`
  - erstellt einen neuen Kontakt in `contacts_`
  - Der User-Pointer `contact` soll als _Key_ in der Map `contacts_` eingefügt werden.
  - Abhängig von `type` wird ein Objekt der jeweiligen abgeleiteten Klasse von `Cipher` mit dem `key` erstellt. Ein Pointer auf dieses Objekt wird in der Map `contacts_` als _Value_ (zum _Key_ `contact`) hinzugefügt.
- `bool sendMessage(const std::string& recipient, const std::string& filename, const std::string& plain_text) const`
  - siehe Erklärung weiter unten
- `bool readMessage(const std::string& filename) const`
  - siehe Erklärung weiter unten

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.

---

## Messages

In diesem Abschnitt werden die Nachrichten beschrieben, die zwischen Benutzer:innen ausgetauscht werden können.

Generell besteht eine Nachricht aus folgenden Informationen:

- Empfänger:in der Nachricht (Recipient)
- Autor:in der Nachricht (Sender)
- Inhalt der Nachricht (Ciphertext)

Folgendes Format ist dabei einzuhalten, wobei Elemente in spitzen Klammern Platzhalter sind:
```
Recipient: <username of recipient>\n
Sender: <username of sender>\n
<encrypted content of the message>\n
```

Üblicherweise würden Nachrichten an einen Server gesendet. Um dies zu vereinfachen, werden in A1 alle Nachrichten lediglich als Textdateien abgespeichert.

### Beispiel

Folgende Nachricht wurde mit dem ASCII-Cipher verschlüsselt:
```
Recipient: Michi\n
Sender: David\n
77 77 78\n
```

Folgende Nachricht wurde mit dem Caesar-Cipher verschlüsselt:
```
Recipient: Aleks\n
Sender: Alex\n
ZLUASFE\n
```

### Beschreibung von `User::sendMessage`

`bool sendMessage(const std::string& recipient, const std::string& filename, const std::string& plain_text) const`

Zu Beginn soll die durch den Parameter `filename` angebene Datei geöffnet werden. Falls dies nicht erfolgreich war, soll `false` zurückgegeben werden.

Anschließend soll überprüft werden, ob die:der Empfänger:in auch wirklich ein Kontakt der:des aktuellen Benutzerin:Benutzers ist. Falls das nicht der Fall ist, soll `false` zurückgeben werden.

Wenn die Datei geöffnet werden konnte und die:der Benutzer:in an einen eigenen Kontakt schreiben möchte, soll der `plain_text` mit dem zu `recipient` gehörenden Verschlüsselungsalgorithmus verschlüsselt werden. Damit kann nun die vollständige Nachricht im oben genannten Format generiert werden und in die geöffnet Datei gespeichert werden. Außerdem soll die vollständige Nachricht auch ausgegeben werden, wozu die Funktion `IO::printEncryptedMessage(const std::string& encrypted_message)` zu verwenden ist. Damit war das Absenden erfolgreich und die Methode `sendMessage` gibt `true` zurück.

### Beschreibung von `User::readMessage`

`bool readMessage(const std::string& filename) const`

Zu Beginn soll die durch den Parameter `filename` angebene Datei geöffnet werden. Falls dies nicht erfolgreich war, soll `false` zurückgegeben werden.

Anschließend soll überprüft werden, ob die:der aktuelle Benutzer:in auch die:der Empfänger:in der Nachricht ist und ob die:der Absender:in auch ein Kontakt der:des aktuellen Benutzerin:Benutzers ist. Falls dies nicht der Fall ist, soll ebenso `false` zurückgegeben werden.

Nun soll der Ciphertext mit dem zur:zum Absender:in gehörigen Verschlüsselungsalgorithmus wieder entschlüsselt werden. Anschließend sollen Empfänger:in, Absender:in und der Klartext ausgegeben werden, wozu die Funktion `IO::printDecryptedMessage(const std::string& recipient, const std::string& sender, const std::string& plain_text)` verwendet werden soll.

Damit war das Lesen der Nachricht erfolgreich und es kann `true` zurückgegeben werden.

> Hinweis: Es kann davon ausgegangen werden, dass das Format von Nachrichten (Dateien) korrekt ist, falls diese geöffnet werden können. Weiters kann davon ausgegangen werden, dass der richtige Verschlüsselungsalgorithmus verwendet worden ist, falls Absender:in und Empfänger:in erfolgreich überprüft wurden.
