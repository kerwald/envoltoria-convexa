# Variáveis de Configuração
CXX = g++

# CXXFLAGS: Adicionamos -Ip8g para que o compilador procure cabeçalhos na pasta 'p8g'.
CXXFLAGS = -std=c++11 -Wall -Ip8g

# LDFLAGS: Adicionamos -Lp8g para que o linker procure bibliotecas na pasta 'p8g'.
LDFLAGS = -Lp8g

LIBS = -lp8g++
TARGET = main.exe

# ADICIONAMOS Unidade.cpp aqui
SRCS = main.cpp Unidade.cpp 

# A lista de objetos é gerada a partir da lista de fontes
OBJS = $(SRCS:.cpp=.o)

# Regra padrão: compila o alvo principal
.PHONY: all
all: $(TARGET)

# Regra para linkar e criar o executável final
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "Projeto compilado com sucesso! Executavel criado: $(TARGET)"

# Regra para compilar os arquivos fonte (.cpp) em arquivos objeto (.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para executar o programa
.PHONY: run
run: all
	./$(TARGET)

# Regra para limpar os arquivos gerados (versão para Windows)
.PHONY: clean
clean:
	@echo "Limpando arquivos gerados..."
	del /f /q $(TARGET) $(OBJS) 2>nul || (exit 0)
	@echo "Limpeza concluida."