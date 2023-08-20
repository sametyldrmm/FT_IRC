# Derleyici
CC := g++

# Derleme seçenekleri
CFLAGS := -Wall -Wextra -Werror -std=c++98 -g 

# Derleme hedefi
TARGET := ft_irc

# Kaynak dosyaları
SOURCES := src/main.cpp src/server.cpp  src/getMessage.cpp src/utils.cpp src/users.cpp src/channel.cpp src/error.cpp

# Nesne dosyaları
OBJECTS := $(SOURCES:.cpp=.o)

# Kural: Tüm hedef
all: $(TARGET)


# Kural: Nesne dosyalarını oluştur
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Kural: Programı derle ve oluşturulan dosyayı taşı
$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

# Temizleme işlemi
clean:
	rm -f $(OBJECTS) $(TARGET) 

re : clean all
# PHONY hedefleri
.PHONY: all clean re