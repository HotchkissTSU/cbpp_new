#ifndef CBPP_CSTRING_H
#define CBPP_CSTRING_H

#include <cstddef>

namespace cbpp {
    class CString {
        private:
            char* m_sData = nullptr;
            size_t m_iLength = 0;

            void Nullify();
            
        public:
            // Конструкторы
            CString();
            CString(size_t iLength);
            CString(const char* sSource);
            CString(const CString& Other);
            CString(CString&& Other) noexcept;
            
            // Деструктор
            ~CString();
            
            // Операторы присваивания
            CString& operator=(const char* sOther);
            CString& operator=(const CString& Other);
            CString& operator=(CString&& Other) noexcept;
            
            // Доступ к данным
            const char* String() const;
            char* Pointer();
            
            // Размер
            size_t Length() const;
            size_t LengthUpdate();
            
            // Проверка
            bool IsValid() const;
            
            // Операторы доступа по индексу
            char& operator[](size_t iIndex);
            const char& operator[](size_t iIndex) const;
            char& At(size_t iIndex);
            const char& At(size_t iIndex) const;
            
            // Операторы сравнения
            bool operator==(const char* sOther) const;
            bool operator==(const CString& Other) const;
            bool operator<(const char* sOther) const;
            bool operator<(const CString& Other) const;
            bool operator>(const char* sOther) const;
            bool operator>(const CString& Other) const;
            
            // Операторы конкатенации
            CString& operator+=(const CString& Other);
            CString& operator+=(char iOther);
            
            // Приведения типов
            operator const char*() const;
            operator char*();
            operator bool() const;
            
            // Дружественные операторы
            friend CString operator+(const CString& A, const CString& B);
            friend CString operator+(const CString& A, const char* B);
    };

    // Внешние операторы
    CString operator+(const CString& A, const CString& B);
    CString operator+(const CString& A, const char* B);
}

#endif
