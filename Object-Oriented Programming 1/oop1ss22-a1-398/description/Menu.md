# Menü





**ACHTUNG**, diese Anleitung dient nur als zusätzliche Erklärung des Programmablaufs. Alles in diesem Dokument Angeführte ist bereits im Framework implementiert.

Für den Programmablauf ist großteils die Funktion `menu` zuständig. In ihr werden

- das Menü ausgegeben,
- Eingaben entgegengenommen und
- auf diese reagiert.

Die meisten Befehle setzen Informationen zur Konfigurationsdatei und der:dem aktuellen Benutzer:in voraus. Diese werden in Form von zwei Objekten an die Funktion `menu` übergeben:

- `config` (vom Typ `Config&`)
  - stellt die Konfigurationsdatei dar
- `active_user` (vom Typ `User*`)
  - repräsentiert die:den aktuell eingeloggte:n Benuter:in

 

Im Menü gibt es fünf Optionen, aus welchen ein:e eingeloggte:r Benutzer:in wählen kann:

- [add](#add-befehl)
  - fügt einen neuen Kontakt hinzu
- [encrypt](#encrypt-befehl)
  - verschlüsselt eine Nachricht und speichert sie zusammen mit Informationen zu Empfänger:in und Absender:in in eine Datei
- [decrypt](#decrypt-befehl)
  - öffnet eine Datei und entschlüsselt die darin befindliche Nachricht
- [show](#show-befehl)
  - zeigt die aktuellen Kontakte an
- [quit](#quit-befehl)
  - beendet das Programm

Mit Ausnahme von `quit` gelangt die:der Benutzer:in nach jedem Befehl wieder zurück zur Menüauswahl.





## Add-Befehl

Wird der Befehl `a` eingegeben, wird der:dem aktuellen Benutzer:in ein neuer Kontakt hinzugefügt. Dieser neue Kontakt muss bereis als registrierte:r Benutzer:in in der Konfigurationsdatei vorhanden sein. Wird ein neuer Kontakt angelegt, so erfolgt das immer in beide Richtungen. Wenn also der Benutzer Donald den Benutzer Joe als Kontakt hinzufügt, dann ist der Kontakt Joe auch automatisch bei Benutzer Donald einzutragen.

Zunächst wird nach dem Namen des Kontakts gefragt:

```
Who would you like to add as a contact?\n
```

Beim Namen des Kontakts, der durch die:den Benutzer:in eingegeben wird, wird überprüft,

- ob der gewünschte Kontakt als registrierte:r Benutzer:in in der Konfigurationsdatei existiert,
- ob der eingegebene Name nicht der eigene Name ist (man kann sich selbst keine Nachrichten schicken) und
- ob der gewünschte Kontakt nicht bereits ein Kontakt ist.

Wenn eine der oben genannten Überprüfungen ein Problem feststellt, wird die Meldung

```
Error: you cannot add this user as a contact!\n
```

ausgegeben und die Eingabe des hinzuzufügenden Kontakts erfolgt erneut. 

Wenn bei den Überprüfungen kein Problem festgestellt wurde, wird nach der Art der Verschlüsselung gefragt:

```
What cipher would you like to use?\n
```

Zuletzt wird bei beiden betroffenen Benutzer:innen der Kontakt hinzugefügt. Dies funktioniert mit Hilfe der Methode `addContact` der `User`-Klasse. Der Key wird mithilfe der Funktion `Utils::deriveKey` berechnet. 

Da sich nun die Konfiguration geändert hat, wird noch die bool-Variable `modified_` in `Config` auf `true` gesetzt, sodass beim Schließen des Programms die Konfiguration neu in die Datei geschrieben wird. 



### Beispiel

In diesem Beispiel gibt es keinen registrierten Benutzer namens `Aleks`. Der Benutzer `Max` erfüllt die Kriterien, um als Kontakt hinzugefügt zu werden.

```
Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > a
Who would you like to add as a contact?
  > Aleks
Error: you cannot add this user as a contact!
  > Max
What cipher would you like to use?
  > falscheEingabe
Error: invalid cipher!
  > ASCII


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



------

## Encrypt-Befehl

Wird der Befehl `e` eingegeben, wird eine Nachricht verschlüsselt. Zunächst wird folgende Meldung ausgegeben:

```
Who would you like to write to?\n
```

Danach wird der Name eines Kontakts eingeben, welchem eine Nachricht übermittelt werden soll.

Als nächstes wird nach dem Namen der Datei gefragt, in der die verschlüsselte Nachricht abgespeichert werden soll:

```
What should be the (file)name of your message?\n
```

Nach der Ausgabe der Eingabeaufforderung

```
Enter your message now:\n
```

folgt die Eingabe der zu verschlüsselnden Nachricht. Diese Nachricht wird zusammen mit Informationen über Empfänger:in und Absender:in an die (zu implementierende) `sendMessage`-Methode des `active_user` übergeben, welche das Senden durchführt.


### encryption-Beispiel

In diesem Beispiel gibt es keinen registrierten Benutzer namens `Aleks`. Der Benutzer `Max` ist bereits als Kontakt hinzugefügt und verwendet die ASCII Verschlüsselung. Die Nachricht lautet: "Hello Max how are you".

```
Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > e
Who would you like to write to?
  > Aleks
Error: invalid contact!
  > Max
What should be the (file)name of your message?
  > secret_filename_for_max.txt
Enter your message now:
  > Hello Max how are you

Sending ...
Recipient: Max
Sender: David
67 64 71 71 74 72 60 83 67 74 82 60 77 64 84 74 80


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



------

## Decrypt-Befehl

Wird der Befehl `d` eingegeben, wird zunächst folgende Meldung ausgegeben:

```
Which message would you like to read?\n
```

Danach soll die:der Benutzer:in den Namen der Datei angeben, die sie:er lesen möchte. Nach dieser Eingabe wird die (zu implementierende) `readMessage`-Methode des `active_user` aufgerufen. Diese entschlüsselt die Nachricht mit dem hinterlegten Key und gibt diese aus.

### decryption-Beispiel

In diesem Beispiel liest der Benutzer `Max` die zuvor verschlüsselte Nachricht von `David`. Es gibt die Datei `wrong_file.txt` nicht, da die Datei `secret_filename_for_max.txt` heißt. Nach der Entschlüsselung wird der Text "HELLOMAXHOWAREYOU" von der Methode `readMessage` des `active_user` ausgegeben. 

```
Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > d
Which message would you like to read?
  > wrong_file.txt
Error: cannot open message!


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > d
Which message would you like to read?
  > secret_filename_for_max.txt

Reading ...
Recipient: Max
Sender: David
HELLOMAXHOWAREYOU


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



------

## Show-Befehl

Hier werden die aktuellen Kontakte von `active_user` in alphabetischer Reihenfolge angezeigt.



### show-Beispiel

In diesem Beispiel hat der Benutzer David zwei Kontakte. Um mit dem Kontakt Michi kommunizieren zu können, wird die Caesar-Verschlüsselung verwendet. Um mit dem Kontakt Max kommunizieren zu können, wird die ASCII-Verschlüsselung verwendet. 

```
Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > s
Contacts of user: David
            Max | ASCII
          Michi | CAESAR


Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > 
```



------

## Quit-Befehl

Wenn der Befehl `q` eingegeben wurde, wird zunächst folgende Meldung ausgegeben werden:

```
Bye!\n
```

Danach wird noch die Methode (zu implementierende) `updateConfigFile` aus der `Config`-Klasse aufgerufen.

### quit-Beispiel

```
Choose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit
  > q
Bye!
```

