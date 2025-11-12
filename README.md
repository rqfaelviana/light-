# 📘 Fluxo de trabalho

- ***

## 🚀 Objetivo deste README

Guia para **ajudar o grupo a entender o fluxo de trabalho no Git/GitHub**

---

## 🛠️ Passo a Passo para Começar

### 🪄 1. Clonar o repositório (baixar o projeto)

Primeiro, copie o link HTTPS do repositório no GitHub (botão verde “Code”).

No terminal (VSCode, Git Bash etc), digite:

```bash
git clone https://github.com/usuario/nome-do-repositorio.git
```

Entre na pasta clonada:

```bash
cd nome-da-pasta-clonada
```

### ### 🪄 2. O que é uma branch? (e por que usar)

Uma branch é uma cópia seperada do projeto principal (main).
Podem ser feitas alterações na branch criada (cópia da main) sem mexer diretamente no projeto principal.

Porque é útil:

- Evita que duas pessoas editem o mesmo arquivo ao mesmo tempo e causem conflitos.

- Permite que coisas possam ser testadas sem interferir nada no projeto principal.

- Mantém o código principal estável.

Exemplo:

- Lisa cria a branch `feature/pagina` e trabalha só nessa parte.
- Jean cria a branch `feature/estilos-css` e mexe apenas no css.
- Depois, os dois juntam(fazem merge) com a main sem apagar o trabalho um do outro. (Essa parte do merge eu posso fazer ou demonstrar na prática o que é e como fazer).

### ### 🪄 3. Criar uma nova branch

Crie sempre uma branch com o nome da funcionalidade que vai trabalhar:

```bash
git checkout -b feature/nome-da-sua-branch
```

Depois desse comando, você já está dentro da branche e pode codar à vontade.

### ### 🪄 4. Fazer alterações

Depois que você fizer as modificações necessárias (HTML, JS, CSS, adição de imagens, etc).

Salva tudo `Ctrl + S`

### ### 🪄 5. Adicionar e confirmar (commit) as mudanças

- 1. Ver os arquivos modificados:

```bash
git status
```

- 2. Adicionar os arquivos modificados:

```bash
git add .
```

Se foi apenas um arquivo modificado, faça:

```bash
git add nome exato do arquivo que foi modificado (index.html)
```

- 3. Faça um commit com uma mensagem breve e descritiva:

```bash
git commit -m "Descrição do que foi feito"
```

### ### 🪄 6. Enviar (push) suas alterações para o Github

```bash
git push origin (nome da branch que você criou)
```

### ### 🪄 7. Atualizar o projeto local (do seu pc)

Antes de começar a codar, sempre atualize a versão do seu computador dando o comando:

```bash
git pull origin main
```

Por que?
Evita conflitos de arquivo e garante que você está mexendo na versão mais atualizada do site.
