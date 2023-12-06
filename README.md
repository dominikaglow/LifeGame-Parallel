# LifeGame-Parallel

**Idea:**

Projekt dotyczy implementacji równoległej symulacji podobnej do Gry w życie. Zmienione zasady gry obejmują wprowadzenie dodatkowego czynnika, jakim jest zanieczyszczenie środowiska. Zanieczyszczenia wpływają na przeżywalność komórek, co wprowadza dodatkowe wyzwania i złożoność do symulacji.

**Klasy:**

**LifeSequentialImplementation:**

Klasa implementuje sekwencyjną wersję symulacji.
Metody klasy obejmują podstawowe operacje związane z ruchem i zmianą stanu komórek w jednym procesie.


**LifeParallelImplementation:**

Klasa rozszerza klasę Life i implementuje wersję równoległą symulacji za pomocą MPI.
Metody klasy są zoptymalizowane pod kątem przyspieszenia obliczeń przy użyciu wielu procesów.


**Cechy projektu:**

Wersja równoległa i sekwencyjna dają identyczne wyniki.

Wszystkie procesy uczestniczą w obliczeniach, co przyspiesza rachunek.
Program działa efektywnie, skracając czas potrzebny do uzyskania efektu w miarę dostępności kolejnych rdzeni/procesorów.
