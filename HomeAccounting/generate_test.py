#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import struct
import os

def write_size(f, value):
    """Write size_t (8 bytes on 64-bit)"""
    f.write(struct.pack('Q', value))

def write_string(f, s):
    """Write string with length prefix"""
    encoded = s.encode('utf-8')
    write_size(f, len(encoded))
    f.write(encoded)

def write_date(f, day, month, year):
    """Write date as 3 integers"""
    f.write(struct.pack('iii', day, month, year))

def write_double(f, value):
    """Write double"""
    f.write(struct.pack('d', value))

def write_income(f, name, category, day, month, year, amount, source):
    """Write income transaction (type 0)"""
    f.write(struct.pack('i', 0))  # type = 0 for income
    write_size(f, 0)  # ID (will be ignored, auto-generated)
    write_string(f, name)
    write_string(f, category)
    write_date(f, day, month, year)
    write_double(f, amount)
    write_string(f, source)

def write_expense(f, name, category, day, month, year, amount, where):
    """Write expense transaction (type 1)"""
    f.write(struct.pack('i', 1))  # type = 1 for expense
    write_size(f, 0)  # ID (will be ignored, auto-generated)
    write_string(f, name)
    write_string(f, category)
    write_date(f, day, month, year)
    write_double(f, amount)
    write_string(f, where)

def generate_test_data():
    filename = 'homeaccounting.dat'
    
    # Backup existing file if it exists
    if os.path.exists(filename):
        backup_name = filename + '.backup'
        if os.path.exists(backup_name):
            os.remove(backup_name)
        os.rename(filename, backup_name)
        print(f'Создан бэкап: {backup_name}')
    
    with open(filename, 'wb') as f:
        # Доходы
        write_income(f, 'Зарплата', 'Доход', 1, 11, 2024, 85000.0, 'Работа')
        write_income(f, 'Премия', 'Доход', 15, 11, 2024, 25000.0, 'Работа')
        write_income(f, 'Фриланс проект', 'Доход', 10, 11, 2024, 35000.0, 'Фриланс')
        write_income(f, 'Возврат долга', 'Доход', 5, 11, 2024, 15000.0, 'Друзья')
        write_income(f, 'Продажа старой техники', 'Доход', 3, 11, 2024, 8500.0, 'Авито')
        write_income(f, 'Дивиденды', 'Доход', 20, 10, 2024, 12000.0, 'Инвестиции')
        
        # Расходы - Продукты
        write_expense(f, 'Продукты', 'Еда', 2, 11, 2024, 4500.0, 'Пятёрочка')
        write_expense(f, 'Продукты', 'Еда', 8, 11, 2024, 5200.0, 'Перекрёсток')
        write_expense(f, 'Продукты', 'Еда', 16, 11, 2024, 3800.0, 'Магнит')
        write_expense(f, 'Продукты', 'Еда', 23, 10, 2024, 4100.0, 'Пятёрочка')
        
        # Расходы - Рестораны
        write_expense(f, 'Обед в кафе', 'Еда', 4, 11, 2024, 1200.0, 'Кофемания')
        write_expense(f, 'Ужин в ресторане', 'Еда', 12, 11, 2024, 3500.0, 'Тануки')
        write_expense(f, 'Доставка еды', 'Еда', 18, 11, 2024, 1800.0, 'Яндекс.Еда')
        write_expense(f, 'Кофе', 'Еда', 6, 11, 2024, 350.0, 'Starbucks')
        write_expense(f, 'Бизнес-ланч', 'Еда', 25, 10, 2024, 450.0, 'Теремок')
        
        # Расходы - Транспорт
        write_expense(f, 'Бензин', 'Транспорт', 6, 11, 2024, 3000.0, 'Роснефть')
        write_expense(f, 'Метро', 'Транспорт', 7, 11, 2024, 500.0, 'Тройка')
        write_expense(f, 'Такси', 'Транспорт', 14, 11, 2024, 850.0, 'Яндекс.Такси')
        write_expense(f, 'Парковка', 'Транспорт', 20, 11, 2024, 300.0, 'Городская парковка')
        write_expense(f, 'Каршеринг', 'Транспорт', 28, 10, 2024, 1200.0, 'Яндекс.Драйв')
        
        # Расходы - Коммунальные услуги
        write_expense(f, 'Электричество', 'Коммунальные', 1, 11, 2024, 2500.0, 'Мосэнерго')
        write_expense(f, 'Интернет', 'Коммунальные', 1, 11, 2024, 800.0, 'Ростелеком')
        write_expense(f, 'Вода', 'Коммунальные', 1, 11, 2024, 1200.0, 'Мосводоканал')
        write_expense(f, 'Отопление', 'Коммунальные', 1, 11, 2024, 3500.0, 'ЖКХ')
        write_expense(f, 'Мобильная связь', 'Коммунальные', 5, 11, 2024, 600.0, 'МТС')
        
        # Расходы - Развлечения
        write_expense(f, 'Кино', 'Развлечения', 9, 11, 2024, 1000.0, 'Каро Фильм')
        write_expense(f, 'Концерт', 'Развлечения', 13, 11, 2024, 4500.0, 'Крокус Сити Холл')
        write_expense(f, 'Подписка Netflix', 'Развлечения', 5, 11, 2024, 990.0, 'Netflix')
        write_expense(f, 'Боулинг', 'Развлечения', 17, 11, 2024, 2500.0, 'Космик')
        write_expense(f, 'Театр', 'Развлечения', 22, 10, 2024, 2800.0, 'МХАТ')
        write_expense(f, 'Подписка Spotify', 'Развлечения', 10, 11, 2024, 299.0, 'Spotify')
        
        # Расходы - Здоровье
        write_expense(f, 'Аптека', 'Здоровье', 11, 11, 2024, 1500.0, '36.6')
        write_expense(f, 'Стоматолог', 'Здоровье', 15, 11, 2024, 8000.0, 'Клиника Smile')
        write_expense(f, 'Спортзал', 'Здоровье', 1, 11, 2024, 3500.0, 'World Class')
        write_expense(f, 'Массаж', 'Здоровье', 19, 10, 2024, 2500.0, 'СПА-центр')
        write_expense(f, 'Витамины', 'Здоровье', 8, 11, 2024, 1200.0, 'Аптека')
        
        # Расходы - Одежда
        write_expense(f, 'Куртка', 'Одежда', 19, 11, 2024, 12000.0, 'Zara')
        write_expense(f, 'Кроссовки', 'Одежда', 21, 11, 2024, 8500.0, 'Спортмастер')
        write_expense(f, 'Джинсы', 'Одежда', 10, 11, 2024, 4500.0, 'H&M')
        write_expense(f, 'Футболки', 'Одежда', 27, 10, 2024, 2500.0, 'Uniqlo')
        write_expense(f, 'Ботинки', 'Одежда', 15, 10, 2024, 9500.0, 'Ecco')
        
        # Расходы - Образование
        write_expense(f, 'Курсы программирования', 'Образование', 1, 11, 2024, 15000.0, 'Skillbox')
        write_expense(f, 'Книги', 'Образование', 7, 11, 2024, 2500.0, 'Лабиринт')
        write_expense(f, 'Онлайн-курс', 'Образование', 12, 10, 2024, 5000.0, 'Coursera')
        write_expense(f, 'Учебные материалы', 'Образование', 20, 10, 2024, 1500.0, 'Буквоед')
        
        # Расходы - Подарки
        write_expense(f, 'Подарок маме', 'Подарки', 22, 11, 2024, 5000.0, 'Золотое яблоко')
        write_expense(f, 'Цветы', 'Подарки', 14, 11, 2024, 1500.0, 'Флора-экспресс')
        write_expense(f, 'Подарок другу', 'Подарки', 18, 10, 2024, 3500.0, 'DNS')
        
        # Расходы - Разное
        write_expense(f, 'Химчистка', 'Разное', 8, 11, 2024, 1200.0, 'Диана')
        write_expense(f, 'Стрижка', 'Разное', 16, 11, 2024, 1800.0, 'Барбершоп')
        write_expense(f, 'Ремонт телефона', 'Разное', 24, 10, 2024, 4500.0, 'Apple Store')
        write_expense(f, 'Бытовая химия', 'Разное', 13, 11, 2024, 1100.0, 'Ашан')
        write_expense(f, 'Канцтовары', 'Разное', 29, 10, 2024, 850.0, 'Комус')
    
    # Calculate statistics
    file_size = os.path.getsize(filename)
    print(f'\n✓ Файл {filename} успешно создан')
    print(f'✓ Размер файла: {file_size} байт')
    print(f'✓ Создано транзакций: 56')
    print(f'  - Доходов: 6')
    print(f'  - Расходов: 50')
    print(f'\nОбщая статистика:')
    
    total_income = 85000 + 25000 + 35000 + 15000 + 8500 + 12000
    total_expense = (4500 + 5200 + 3800 + 4100 +  # Продукты
                     1200 + 3500 + 1800 + 350 + 450 +  # Рестораны
                     3000 + 500 + 850 + 300 + 1200 +  # Транспорт
                     2500 + 800 + 1200 + 3500 + 600 +  # Коммунальные
                     1000 + 4500 + 990 + 2500 + 2800 + 299 +  # Развлечения
                     1500 + 8000 + 3500 + 2500 + 1200 +  # Здоровье
                     12000 + 8500 + 4500 + 2500 + 9500 +  # Одежда
                     15000 + 2500 + 5000 + 1500 +  # Образование
                     5000 + 1500 + 3500 +  # Подарки
                     1200 + 1800 + 4500 + 1100 + 850)  # Разное
    
    balance = total_income - total_expense
    
    print(f'  Доходы: {total_income:,.2f} руб.')
    print(f'  Расходы: {total_expense:,.2f} руб.')
    print(f'  Баланс: {balance:,.2f} руб.')
    print(f'\nТеперь запустите приложение HomeAccounting для просмотра данных!')

if __name__ == '__main__':
    generate_test_data()