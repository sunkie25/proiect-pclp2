# Proiect PCLP2 - Slot Machine
![slot](https://i.imgur.com/vwnRe7m.png)

## 1. Prezentarea proiectului 

Proiectul este un joc de tip **Slot Machine** realizat in limbajul C, folosind biblioteca OpenGL/GLUT pentru interfata grafica. Aplicatia afiseaza o fereastra cu dimensiunea fixa de `1152 x 640` pixeli, in care utilizatorul poate alege o miza, poate porni rotirea simbolurilor si poate schimba tema vizuala a jocului.

Tabla de joc are **3 randuri si 5 coloane**, adica 15 pozitii in total. La fiecare rotire, simbolurile afisate sunt schimbate aleator, iar dupa terminarea animatiei se verifica daca exista combinatii castigatoare pe una dintre cele 5 linii posibile. Daca jucatorul castiga, suma castigata se adauga la sold. Daca nu exista castig, miza ramane pierduta, deoarece aceasta este scazuta inainte de pornirea rotirii.

Jucatorul are la dispozitie 4 mize:

| Miza | Valoare |
| --- | ---: |
| BET 5 | 5 coins |
| BET 10 | 10 coins |
| BET 50 | 50 coins |
| BET 100 | 100 coins |

In cod este activat `TEST_MODE`, deci soldul initial folosit la rulare este `500 coins`. Daca se dezactiveaza `TEST_MODE`, soldul initial devine `10000 coins`.

## 2. Elemente principale ale jocului

Interfata este formata din zona principala a slotului, butoanele de miza, butonul `SPIN`, butonul `THEME`, soldul jucatorului si zona de notificari. Notificarile sunt folosite pentru mesaje precum castig, pierdere, fonduri insuficiente sau incercarea de a schimba miza in timpul rotirii.

Jocul foloseste si cateva fisiere externe:

| Fisier | Rol |
| --- | --- |
| `main.c` | contine logica jocului, desenarea interfetei si tratarea click-urilor |
| `offsets.txt` | contine coordonatele celor 15 casete din slot |
| `theme.txt` | salveaza tema curenta, ca sa fie pastrata intre rulari |
| `logs.txt` | salveaza istoricul sesiunilor si rezultatele rotirilor |

Exista **3 teme vizuale**, selectabile din butonul `THEME`. Temele modifica paleta de culori folosita pentru fundal, butoane, simboluri si text. In proiect sunt gandite ca variante de tip classic, cartoon si neon, astfel incat jocul sa nu arate mereu la fel.

## 3. Simboluri si valori

Slotul foloseste 5 tipuri de simboluri. Fiecare simbol are un multiplicator propriu, folosit la calcularea castigului:

| Simbol | Constanta in cod | Multiplicator simbol |
| --- | --- | ---: |
| TEST | `ITEM_TYPE_TEST` | x10 |
| CASH | `ITEM_TYPE_CASH` | x2 |
| DIAMOND | `ITEM_TYPE_DIAMOND` | x5 |
| ROMB | `ITEM_TYPE_ROMB` | x7 |
| BAR | `ITEM_TYPE_BAR` | x4 |

Simbolurile sunt desenate manual cu primitive OpenGL:

| Simbol | Cum este desenat |
| --- | --- |
| TEST | doua dreptunghiuri care formeaza un simbol simplu |
| CASH | mai multe dreptunghiuri, asezate ca o forma de bani |
| DIAMOND | doua forme geometrice, un triunghi si un poligon |
| ROMB | un poligon in forma de romb |
| BAR | un dreptunghi cu textul `BAR` in interior |

Functiile care deseneaza simbolurile sunt separate: `drawTestSymbol`, `drawCashSymbol`, `drawDiamondSymbol`, `drawRombSymbol` si `drawBarSymbol`. Functia `drawSlotSymbol` primeste tipul simbolului si apeleaza functia potrivita.

## 4. Linii de castig

Tabla are 5 linii de castig. O linie este castigatoare daca primele 3, primele 4 sau toate cele 5 simboluri de pe acea linie sunt identice. Castigurile se verifica de la stanga la dreapta.

Liniile sunt:

| Linie | Pozitii verificate |
| --- | --- |
| Linia 1 | randul de sus |
| Linia 2 | randul din mijloc |
| Linia 3 | randul de jos |
| Linia 4 | diagonala de sus-stanga spre jos si apoi spre sus |
| Linia 5 | diagonala de jos-stanga spre sus si apoi spre jos |

In cod, liniile sunt memorate in matricea `linePositions`, din functia `updateWinningItems`:

```c
{0, 3, 6, 9, 12}
{1, 4, 7, 10, 13}
{2, 5, 8, 11, 14}
{0, 4, 8, 10, 12}
{2, 4, 6, 10, 14}
```

Functia `getLineComboLength` determina lungimea combinatiei. Daca primele 3 simboluri sunt egale, combinatia are minimum 3 simboluri. Daca si al patrulea simbol este egal, combinatia devine de 4. Daca si al cincilea simbol este egal, combinatia devine de 5.

## 5. Calcularea castigurilor

Castigul este calculat cu formula:

```text
castig = miza * multiplicator_simbol * multiplicator_lungime
```

Multiplicatorul de lungime depinde de cate simboluri identice sunt pe linie:

| Lungime combinatie | Multiplicator lungime |
| --- | ---: |
| 3 simboluri | x1 |
| 4 simboluri | x5 |
| 5 simboluri | x10 |

Prin urmare, multiplicatorii totali fata de miza sunt:

| Simbol | 3 simboluri | 4 simboluri | 5 simboluri |
| --- | ---: | ---: | ---: |
| TEST | x10 | x50 | x100 |
| CASH | x2 | x10 | x20 |
| DIAMOND | x5 | x25 | x50 |
| ROMB | x7 | x35 | x70 |
| BAR | x4 | x20 | x40 |

Exemplu: daca miza este `10 coins` si jucatorul obtine 4 simboluri `DIAMOND`, castigul este:

```text
10 * 5 * 5 = 250 coins
```

Daca exista mai multe linii castigatoare in aceeasi rotire, castigurile se aduna in `slotWinAmount`. Pozitiile castigatoare sunt marcate in vectorul `slotItemWon`, iar casetele respective sunt colorate diferit pe ecran.

## 6. Fluxul unei rotiri

Cand utilizatorul apasa butonul `SPIN`, functia `handleSpinButtonClick` verifica mai intai daca slotul nu este deja in animatie. Apoi verifica daca exista o miza selectata si daca soldul este suficient pentru acea miza. Daca totul este valid, miza este scazuta din sold, iar jocul genereaza o noua combinatie de simboluri.

Rotirea are doua etape:

1. `shuffleVisibleSymbols` pune simboluri aleatoare pe tabla, pentru efectul de rotire.
2. `generateSpinResult` genereaza rezultatul final care va fi afisat treptat.

Animatia este controlata prin `glutTimerFunc`. Functia `revealNextSpinSymbol` afiseaza cate un simbol final la fiecare pas, folosind intarzierea `SPIN_TIMER_DELAY`, setata la `150 ms`. Cand toate cele 15 simboluri au fost afisate, se apeleaza `finishSpin`.

La final, `finishSpin` apeleaza `updateWinningItems`, calculeaza castigul, actualizeaza soldul si afiseaza o notificare. Tot aici se scrie si rezultatul in `logs.txt`, sub forma de mesaj de tip `[WIN]` sau `[LOSE]`.

## 7. Rolul principalelor functii

Functiile pentru fisiere:

| Functie | Rol |
| --- | --- |
| `appendLogMessage` | adauga mesaje in `logs.txt` |
| `loadSavedTheme` | citeste tema curenta din `theme.txt` |
| `saveSelectedTheme` | salveaza tema aleasa in `theme.txt` |
| `loadSlotItemDesigns` | citeste coordonatele casetelor din `offsets.txt` |

Functiile pentru interfata:

| Functie | Rol |
| --- | --- |
| `drawGameScreen` | redeseneaza toata fereastra jocului |
| `drawMachineFrame` | deseneaza rama slotului |
| `drawAllSlotCells` | deseneaza cele 15 casete si simbolurile lor |
| `drawGridLines` | deseneaza liniile care separa randurile si coloanele |
| `drawSpinButton` | deseneaza butonul de pornire |
| `drawThemeButton` | deseneaza butonul de schimbare a temei |
| `drawCashBalance` | afiseaza soldul jucatorului |
| `drawBetButtons` | afiseaza butoanele de miza |
| `drawNotificationMessage` | afiseaza mesajele temporare |

Functiile pentru logica jocului:

| Functie | Rol |
| --- | --- |
| `handleSpinButtonClick` | porneste o rotire daca miza si soldul sunt valide |
| `shuffleVisibleSymbols` | genereaza simboluri temporare pentru inceputul rotirii |
| `generateSpinResult` | genereaza rezultatul final al rotirii |
| `startSpinAnimation` | porneste timer-ul animatiei |
| `revealNextSpinSymbol` | afiseaza progresiv simbolurile finale |
| `finishSpin` | opreste rotirea si trateaza rezultatul |
| `updateWinningItems` | verifica liniile si aduna castigurile |
| `getLineComboLength` | stabileste daca o linie are 3, 4 sau 5 simboluri identice |
| `getLineWinAmount` | calculeaza suma castigata pe o linie |
| `markWinningLine` | marcheaza simbolurile castigatoare pentru evidentiere |

Functiile pentru input:

| Functie | Rol |
| --- | --- |
| `mouseCallback` | primeste click-urile de la GLUT |
| `getClickedBetButton` | verifica daca utilizatorul a apasat o miza |
| `handleBetButtonClick` | schimba miza selectata |
| `handleActionButtonClick` | verifica butoanele `SPIN` si `THEME` |
| `handleThemeButtonClick` | schimba tema curenta |
| `keepWindowFixedSize` | pastreaza fereastra la dimensiunea stabilita |

## 8. Concluzie

Proiectul implementeaza un slot machine complet functional, cu interfata grafica, animatie, mize diferite, teme vizuale, sistem de castiguri si log-uri. Codul este impartit in functii care separa desenarea, logica jocului, citirea fisierelor si tratarea inputului, astfel incat proiectul sa fie mai usor de explicat si de urmarit.

Cele mai importante parti ale proiectului sunt calcularea castigurilor prin multiplicatori, verificarea celor 5 linii de castig si actualizarea interfetei in functie de starea jocului. Prin folosirea fisierelor `theme.txt`, `logs.txt` si `offsets.txt`, proiectul pastreaza si informatii externe, nu doar date din memoria programului.  
![nebunulajokuri](https://i.imgur.com/d0jQc84.jpeg)