# Сборка модуля с ядром nginx

Для сборки модуля с ядром nginx необходимо выполнить команду

```
./configure --add-module=путь/до/модуля/nginx_modul_sber
```
# Описание директивы

Для работы верификации json при регистрации персонажа, необходимо включить следующую директиву:

```
location /check_json {
    character_shem_check on;
}
```

Регулировка работы модуля осуществляется написанием "on", "off" у директивы character_shem_check.

# Валидируемые схемы json

В рамках данного пет-проекта, модуль валидирует следующую схему json:
```
{
    "name": "Aragorn",
    "race": "Human",
    "ch_class": "Ranger",
    "level": 5,
    "hit_points":
    {
        "max": 40,
        "current": 35
    },
    "abilities":
    {
        "strength": 18,
        "dexterity": 13,
        "constitution": 16,
        "intelligence": 12,
        "wisdom": 14,
        "charisma": 11
    },
    "skills": ["Survival", "Analysis", "Stealth"]
}
```
# Пример запроса

Далее будут приведен пример http запросы для оценки работоспособности модуля:

```
curl -X POST http://localhost/check_json \
     -H "Content-Type: application/json" \
     -d '{
          "name": "Aragorn",
          "race": "Human",
          "ch_class": "Ranger",
          "level": 5,
          "hit_points": {
              "max": 40,
              "current": 35
          },
          "abilities": {
              "strength": 18,
              "dexterity": 13,
              "constitution": 16,
              "intelligence": 12,
              "wisdom": 14,
              "charisma": 11
          },
          "skills": ["Survival", "Analysis", "Stealth"]
      }'
```
Ожидаемый ответ:

HTTP/1.1 200 OK

Content-Type: text/plain

Тело: The character is saved