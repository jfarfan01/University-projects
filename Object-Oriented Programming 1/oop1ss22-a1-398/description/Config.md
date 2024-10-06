# Konfiguration

## Inhalt der Konfigurationsdatei

In der Konfigurationsdatei sind folgende Informationen über jede:n Benutzer:in gespeichert:

- Benutzername
- Passwort
- Liste der Kontakte, wobei jeder Listeneintrag (Kontakt) aus folgenden Elementen besteht:
  - Benutzername des Kontakts,
  - Art des Verschlüsselungsalgorithmus, um mit diesem Kontakt kommunzieren zu können sowie
  - Key für die Verschlüsselung.

Benutzernamen und Passwörter dürfen nur aus Großbuchstaben, Kleinbuchstaben und Zahlen bestehen und müssen mindestens ein Zeichen lang sein.

Als Verschlüsselungsalgorithmen stehen `NONE`, `ASCII` und `CAESAR` zur Auswahl. Der Name des Algorithmus muss in genau dieser Form (nur Großbuchstaben) in der Konfigurationsdatei vorkommen.

Als Keys werden in diesem Programm 64-bit Ganzzahlen verwendet. Damit ein Key aus der Konfigurationsdatei gültig ist, muss dieser in Hexadezimalschreibweise genau eine 64-bit Ganzzahl darstellen. Das bedeutet also, ein Key ist eine Zeichenfolge bestehend aus den Zahlen 0 - 9 sowie a - f (Kleinbuchstaben) mit der Länge 16. *Hinweis:* Eine Methode aus der vorgegebenen Klasse `Utils` könnte beim Einlesen hilfreich sein!

Für ein:e Benutzer:in mit drei Kontakten hat der entsprechende Eintrag in der Konfigurationsdatei folgendes Format:

```
Benutzername;Passwort;Kontakt1:Ciphertyp1,Key1;Kontakt2:Ciphertyp2,Key2;Kontakt3:Ciphertyp3,Key3;\n
```

Man beachte hierbei die Zeichen `;`, `:`, und `,` als Trennung der einzelnen Elemente, sowie das abschließende `\n` am Ende des Eintrags.

Abgesehen vom Format der Einträge der Konfigurationsdatei, sind folgende Punkte erforderlich:

1. Es darf keine zwei Einträge in der Konfigurationsdatei mit dem gleichen Benutzernamen geben.
2. Ein:e Benutzer:in darf sich nicht selbst als Kontakt haben.
3. Jeder Kontakt muss selbst ein:e Benutzer:in sein.
4. Jeder Kontakt darf maximal einmal pro Kontaktliste vorkommen.
5. Jeder Kontakt einer:eines Benutzers:Benutzerin muss die:den Benutzer:in selbst wieder als Kontakt haben. (Symmetrie)
6. Die Kontaktliste muss alphabetisch geordnet sein. (Dieser Punkt wird beim Einlesen nicht getestet. Beim Schreiben in das Konfigurationsfile passiert dies aufgrund der Sortierfunktion in der Map automatisch)

Sollte die Konfigurationsdatei von einem der hier angeführten Punkte abweichen, ist sie nicht gültig. Es kann angenommen werden, dass keine Leerzeichen in der Konfigurationsdatei vorkommen. Ist das Konfigurationsfile leer ist es ebenso gültig. 

### Beispiel

Um das Ganze zu verdeutlichen, hier ein Beispiel einer gültigen Konfigurationsdatei.

```
Aleks;SicheresPasswort;David:ASCII,0123456789abcdef;\n
David;BesseresPasswort;Aleks:ASCII,0123456789abcdef;Michi:CAESAR,e53287325a78d90f;\n
Michi;AchEgal12345;David:CAESAR,e53287325a78d90f;\n
```

Man beachte, dass nicht zwingend jede Person mit jeder anderen Person kommunizieren kann. In diesem Beispiel haben sich Aleks und Michi gegenseitig nicht als Kontakt eingespeichert. Man beache weiters, dass in der Kontaktliste von David zuerst Aleks und anschließend Michi vorkommt, da die Kontakte genau so geordnet sein müssen, wie die Benutzer:innen in der Datei.

Weiters noch ein Beispiel einer ungültigen Konfigurationsdatei.

```
Aleks#1;Passwort;David:ASCII,0123456789abc;\n
David;CoolesPasswort;Michi:CIPHER,abcdefghijklmnop;\n
```

Folgende Punkte sind in dieser Konfigurationsdatei nicht gültig:
1. `Aleks#1` ist kein gültiger Benutzername.
2. `Aleks#1` hat `David` als Kontakt aber `David` hat `Aleks#1` nicht als Kontakt.
3. Der Key, der von `Aleks#1` verwendet wird, um Nachrichten an `David` zu senden, besteht nicht aus genau 16 Zeichen.
4. `David` hat `Michi` als Kontakt aber `Michi` ist kein Benutzer in dieser Konfigurationsdatei.
5. `CIPHER` ist kein gültiger Verschlüsselungsalgorithmus
6. Der Key, der von `David` verwendet wird, um Nachrichten an `Michi` zu senden, ist keine Hexadezimalzahl.

---

## Klasse `Config`

> Config.cpp und Config.hpp

Im Sinne der objektorientierten Programmierung soll für die Konfiguration eine Klasse zuständig sein. Dadurch ist es möglich, über klar definierte Schnittstellen auf die Inhalt der Konfigurationsdatei zuzugreifen. Die Klasse soll `Config` heißen und folgende Attribute bzw. Methoden besitzen:

### Attribute

- `file_`
  - die Konfigurationsdatei als `std::fstream`
- `filename_`
  - der Name der Konfigurationsdatei als `std::string`
- `modified_`
  - gibt an ob die Konfigurationsdatei geändert wurde (um sie beim Schließen zu speichern)
  - Datentyp: `bool`
- `users_`
  - ein `std::vector` aus `User*`
  - Hier sollen alle Benutzer:innen (=User) aus der Konfigurationsdatei als Pointer gespeichert werden.

### Konstruktoren

- `Config(const std::string& filename)`
  - öffnet die Datei mit dem Namen `filename` und speichert das File als std::fstream im Attribut `file_`
    - Streams werden am 31.3. in der VO behandelt
  - setzt das Attribut `filename_` auf `filename`
  - setzt das Attribut `modified_` auf false
- Der Copy-Konstruktor soll gelöscht werden.
- Es soll ein geeigneter Destruktor implementiert werden, der falls nötig allokierten Speicher wieder freigibt.

### Methoden

- `void setConfigModified()`
  - setzt das Attribut `modified_` auf `true`
- `bool isOpen() const`
  - gibt zurück, ob `file_` erfolgreich geöffnet werden konnte
- `bool parseFile()`
  - gibt zurück, ob `file_` eine gültige Konfigurationsdatei nach den oben genannten Kriterien ist
  - siehe Hinweise weiter unten
- `bool containsUser(const std::string& name) const`
  - überprüft, ob es eine:n Benutzer:in mit dem Namen `name` in `users_` gibt
- `User* getUser(const std::string& name) const`
  - sucht in `users_` nach einer:einem Benutzer:in mit dem Namen `name`
  - Rückgabewert: 
    - wenn die:der Benutzer:in gefunden wurde, wird ein Pointer auf den entsprechenden `User` zurückgegeben. 
    - ansonsten wird `nullptr` zurückgegeben (`nullptr` ist das C++ Äquivalent zu `NULL`)
- `User* registerUser(const std::string& name, const std::string& password)`
  - fügt eine:n neue:n Benutzer:in mit dem Namen `name` und dem Passwort `password` am Ende des Vectors `users_` hinzu
  - In diesem Fall ändert sich die Konfiguration und es soll `modified_` auf `true` gesetzt werden.
  - Rückgabe: ein Pointer auf die:den neu erstellte:n Benutzer:in
- `User* loginUser(const std::string& name, const std::string& password) const`
  - sucht nach dem User mit dem Namen `name` in `users_` und ruft von diesem die Methode `verifyPassword` auf
  - Rückgabewert:
    - wenn das Passwort stimmt, wird ein Pointer auf den entsprechenden `User` zurückgegeben
    - ansonsten wird `nullptr` zurückgegeben 
- `bool updateConfigFile()`
  - falls `modified_` den Wert `true` besitzt, hat sich die Konfiguration geändert
  - In diesem Fall soll die Konfigurationsdatei überschrieben und `modified_` auf `false` gesetzt werden.
  - siehe Hinweise weiter unten
  - Rückgabewert: `false`, falls die Datei nicht geöffnet werden konnte, ansonsten `true`

> Hinweis: Weitere Attribute und Methoden können zu allen Klassen jederzeit hinzugefügt werden. Dies wird in einigen Fällen hilfreich und notwendig sein.

---

### Was ist nun genau die Aufgabe dieser Klasse?

Es wäre nicht sehr effizient und eher umständlich, wenn für jede der oben geannten Methoden tatsächlich auf die Konfigurationsdatei zugegriffen werden muss. Beispielweise müsste dann jedesmal bei der Passwortüberprüfung von einer:m Benutzer:in, der entsprechende Eintrag erst in der Datei gefunden und ausgelesen werden.

Um dieses Problem zu umgehen, soll die Konfigurationsdatei in der Methode `parseFile` vollständig ausgelesen werden. Dabei wird einerseits überprüft, ob die Konfigurationsdatei gültig ist. Andererseits soll an dieser Stelle der gesamte Inhalt in die interne Datenstruktur `users_` gespeichert werden. `users_` ist ein Vector bzw. eine Liste aus Pointer auf Benutzer:innen (siehe Klasse `User`). Da jede Zeile aus der Konfigurationdatei für genau eine:n Benutzer:in bestehend aus Benutzername, Passwort, Kontakte steht, ist es sinnvoll für jede Zeile ein `User`-Objekt am Heap anzulegen und den Pointer in `users_` abzuspeichern. Weiters besitzt ein Objekt der Klasse `User` Attribute für Namen, Passwort, Kontakte, womit letzendlich der gesamte Eintrag aus der Konfigurationsdatei in einem solchen Objekt abgespeichert werden kann.

Damit ist es ausreichend, ab jetzt nur noch auf `users_` zuzugreifen, da dort alle Informationen aus der Konfigurationsdatei abgespeichert sind. Die Datei selbst braucht ab diesem Moment nicht mehr ausgelesen werden und kann nach dem Einlesen geschlossen werden.

Im Falle, dass sich ein:e neue:r Benutzer:in registriert, oder ein:e Benutzer:in einen Kontakt hinzufügt, ändert sich die Konfiguration. Dies soll durch das Setzen des Attributs `modified_` signalisiert werden. Die Änderung wird zunächst nur intern in der Benutzerliste `users_` übernommen. Im Falle einer Registrierung wird dabei ein neues `User`-Objekt am Heap angelegt. Im Falle des Hinzufügens eines neuen Kontakts werden die Kontaktlisten der betroffenen Benutzer:innen aktualisiert.

Erst wenn das Programm beendet wird (und `modified_` auf `true` gesetzt ist), muss die Änderung tatsächlich in die Konfigurationsdatei übernommen werden. Dies soll in der Methode `updateConfigFile` implementiert werden. Ein möglicher Ansatz ist es, die bestehende Konfigurationsdatei gänzlich mit der Benutzerliste zu überschreiben. Es ist dabei zu beachten, dass alle Kontaktlisten alphabetisch sortiert gespeichert werden. Falls die hier vorgesehene Implementierung umgesetzt wird, erfolgt dies automatisch und muss nicht weiter beachtet werden. Sollte es nicht möglich sein, die Konfigurationsdatei (erneut) zu öffnen, soll die Methode `false` zurückgeben.

### Beispiel

Selbe Konfigurationsdatei wie bereits oben beschrieben:
```
Aleks;SicheresPasswort;David:ASCII,0123456789abcdef;\n
David;BesseresPasswort;Aleks:ASCII,0123456789abcdef;Michi:CAESAR,e53287325a78d90f;\n
Michi;AchEgal12345;David:CAESAR,e53287325a78d90f;\n
```

Die interne Repräsentation soll dann folgendermaßen aussehen (dies ist kein gültiges C++):
```
std::vector<User*> users_{
  [0]: 0x5651c7bfab50 -> User
                         {
                           name_: Aleks,
                           password_: SicheresPasswort,
                           contacts_
                           {
                             users[1], Cipher{ASCII, 0123456789abcdef}
                           }
                         },
  [1]: 0x5651c7bfab30 -> User
                         {
                           name_: David,
                           password_: BesseresPasswort,
                           contacts_
                           {
                             users[0], Cipher{ASCII, 0123456789abcdef}
                             users[2], Cipher{CAESAR, e53287325a78d90f}
                           }
                         },
  [2]: 0x5651c7bfae20 -> User
                         {
                           name_: Michi, 
                           password_: AchEgal12345, 
                           contacts_
                           {
                             users[1], Cipher{CAESAR, e53287325a78d90f}
                           }
                         }
}
```
