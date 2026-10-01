# Custom C Memory Allocato: Heap Manager

Uma implementação didática e customizada do gerenciador de memória dinâmica (`malloc` e `free`) em C. 

O projeto simula a Heap usando um *memory pool* estático e gerencia as alocações via lista encadeada de metadados.

## Destaques da Implementação

- **Header-Based Metadata:** Cada bloco de memória é precedido por um cabeçalho invisível (*Header*) que armazena seu tamanho, estado e ponteiros de navegação.

- **Estratégia First-Fit:** Busca pelo primeiro bloco livre que atenda ao tamanho solicitado.

- **Divisão de Blocos (*Splitting*):** Reduz o desperdício cortando blocos grandes quando a alocação requer menos memória.

- **Coalescência no `free` (*Coalescing*):** Combina automaticamente blocos livres vizinhos para combater a fragmentação externa da Heap.

- **Aritmética de Ponteiros:** Manipulação direta de endereços na RAM sem dependência do SO.
---

## Simulação da Memória RAM
```bash
Simulação da Memória RAM (pool_memoria)
┌──────────────┬────────────────────────┬──────────────┬───────────────┐
│ Header (8 B) │  Dados do Usuário (N B)│ Header (8 B) │ Dados...      │
├──────────────┼────────────────────────┼──────────────┼───────────────┤
│ tam: 32      │ [   payload  ...]      │ tam: 64      │ ...           │
│ livre: 0     │                        │ livre: 1     │               │
└──────────────┴────────────────────────┴──────────────┴───────────────┘
^              ^                        ^
|              └─ Ponteiro que o        └─ Próximo bloco na RAM
└─ Header 1       'my_malloc' retorna
```

---
## Como Compilar e Executar

```bash
# Compilação com flags de rigor do C
gcc -Wall -Wextra -std=c99 mini_allocator.c -o allocator

# Execução
./allocator