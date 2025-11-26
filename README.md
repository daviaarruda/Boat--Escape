## 🐧 Como rodar o Boat Escape no Ubuntu (WSL) — Guia para Iniciantes

Este guia foi feito para qualquer pessoa que nunca usou Ubuntu/WSL antes.  
Ao final, você conseguirá baixar, compilar e rodar o jogo **Boat Escape** no seu computador.

---

# 🧰 1) Instalar o Ubuntu (WSL)

1. No Windows, abra o **Microsoft Store**
2. Pesquise por **Ubuntu**
3. Clique em **Instalar**
4. Após instalar, abra o Ubuntu pelo menu Iniciar

Ele abrirá um terminal preto e pedirá para criar um nome de usuário.  
Digite qualquer nome e senha **(não aparece quando digita, é normal)**.

---

# 🛠️ 2) Instalar o compilador GCC no Ubuntu

No terminal, digite:

```bash
sudo apt update
sudo apt install build-essential
```

Isso instala:

- gcc (compilador C)
- g++ (compilador C++)
- make
- ferramentas essenciais para compilar programas em C

Para confirmar:

```bash
gcc --version
```

Se aparecer a versão, está instalado corretamente.

---

# 📥 3) Baixar o jogo Boat Escape

Existem duas formas:

---

## 🔹 Opção A — Usando Git (recomendado)

Instale o Git:

```bash
sudo apt install git
```

Baixe o projeto:

```bash
git clone https://github.com/daviaarruda/Boat--Escape
```

Entre na pasta:

```bash
cd Boat--Escape
```

---

## 🔹 Opção B — Baixar ZIP do GitHub (para iniciantes)

1. Entre no repositório no GitHub  
2. Clique em **Code → Download ZIP**  
3. Extraia o ZIP na sua área de trabalho  
4. No Ubuntu, acesse a pasta usando `/mnt/c/`, que representa o disco C: do Windows

Exemplo:

```bash
cd /mnt/c/Users/SEU_NOME/Downloads/Boat--Escape
```

---

# 🗂️ 4) Como entrar na pasta do projeto

Se você colocou o jogo no Desktop do Windows:

```bash
cd /mnt/c/Users/SEU_NOME/Desktop/Boat--Escape
```

> 💡 Dica:  
> Tudo do Windows aparece no Ubuntu dentro de `/mnt/c/`.

---

# 🧱 5) Compilar o jogo

Dentro da pasta do projeto, execute:

```bash
gcc src/*.c -Iinclude -o jogo
```

Este comando:

- compila todos os arquivos `.c` da pasta **src/**
- usa os headers da pasta **include/**
- gera o executável **jogo**

Se não aparecer erro, a compilação deu certo.

---

# ▶️ 6) Rodar o jogo

Para jogar, execute:

```bash
./jogo
```

O jogo abrirá diretamente no terminal.  
Use **Q** e **E** para desviar dos obstáculos!

---

# 🎮 Controles do jogo

| Tecla | Função |
|-------|--------|
| **Q** | Move o barco para a esquerda |
| **E** | Move o barco para a direita |
| **ENTER** | Inicia o jogo / Confirma |

---

# 🧩 Estrutura do projeto

```
Boat--Escape/
├─ include/        ← arquivos .h
├─ src/            ← arquivos .c (códigos do jogo)
├─ ranking.txt     ← criado automaticamente
└─ README.md
```

---

# 🔧 Erros comuns e soluções

### ❌ “gcc: command not found”
Você não instalou o compilador.  
Solução:

```bash
sudo apt install build-essential
```

---

### ❌ “Permission denied” ao rodar `./jogo`
Solução:

```bash
chmod +x jogo
```

---

### ❌ Caminho errado ao usar `cd`
Verifique o nome exato da pasta.  
Liste os arquivos com:

```bash
ls
```

---

# 🧑‍💻 Autor

Desenvolvido por **Davi Arruda**  
Projeto acadêmico – 2025
