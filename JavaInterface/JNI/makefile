JAVA_HOME = /usr/lib/jvm/java-17-openjdk-amd64
CC = g++
JAVAC = javac

# Имена и пути
CLASS_NAME = JavaJNI
PACKAGE_DIR = javajni
NATIVE_DIR = JNIdll
LIB_NAME = libJNIdll.so

all: build run

# 1. Генерация заголовочного файла (.h)
# -h $(NATIVE_DIR) — положить заголовок в папку JNIdll
mk_h:
	$(JAVAC) -h $(NATIVE_DIR) $(PACKAGE_DIR)/$(CLASS_NAME).java

# 2. Компиляция C++ библиотеки (.so)
# Используем && чтобы cd сработал для команды компиляции
mk_so:
	cd $(NATIVE_DIR) && \
	$(CC) -std=c++17 -fPIC -shared -o $(LIB_NAME) $(CLASS_NAME).cpp \
		-I$(JAVA_HOME)/include \
		-I$(JAVA_HOME)/include/linux

# 3. Компиляция Java класса
build: mk_h mk_so
	$(JAVAC) $(PACKAGE_DIR)/$(CLASS_NAME).java

# 4. Запуск
# java.library.path указывает, где искать .so библиотеку
run: build
	java -cp . -Djava.library.path=$(NATIVE_DIR) $(PACKAGE_DIR).$(CLASS_NAME)

# Очистка
clean:
	rm -f $(NATIVE_DIR)/*.so $(NATIVE_DIR)/*.h
	rm -f $(PACKAGE_DIR)/*.class
