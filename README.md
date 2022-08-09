# Matrix inverter

#### Este programa realiza a inversão de uma matriz quadrada do tipo double e aplica o refinamento na matriz inversa encontrada. 

Este projeto foi desenvolvido como trabalho semestral para a disciplina de **Introdução à Computação Científica** do curso de **Bacharelado em Ciência da Computação** na **Universidade Federal do Paraná (UFPR)**.


### Instruções de uso

Abra o diretório em seu terminal e rode **`make`** para compilar o programa.
##### Parâmetros:

Abaixo você encontra a lista de parâmetros para a execução do programa.

##### `-e <filename>` 
> Indica o arquivo de entrada do programa. Padrão é stdin.

##### `-s <filename>` 
> Indica o arquivo de saída do programa. Padrão é stdout.

##### `-i <int>`
> Indica o número de iterações do refinamento da matriz inversa. Este parâmetro é **obrigatório**.

##### `-r <int>`
> Caso não queira utilizar um arquivo de entrada, esta opção permite indicar para o programa o tamanho de uma matriz a ser gerada aleatoriamente.

##### `< filename`
> Ao invés do parâmetro **-e**, também podemos usar a entrada padrão do programa para indicar o arquivo de entrada.

##### `> filename`
> Ao invés do parâmetro **-s**, também podemos usar a saída padrão do programa para indicar o arquivo de saída.

##### Formato da entrada:

Abaixo você encontra um exemplo de uma entrada esperada pelo programa. 

```txt
3          // Tamanho da matrix
25 5 1     // Linha 1
64 8 1     // Linha 2
144 12 1   // Linha 3
```

Vale ressaltar que o arquivo deve conter apenas o valor N inteiro para o tamanho da matriz e logo abaixo os N*N valores double representando cada linha da matriz. Os comentários no arquivo acima são apenas explicativos e não devem estar presente.

##### Formato da saída:

Abaixo você encontra um exemplo de uma saída gerada pelo programa. 

```txt
# iter 1: <||2.81743018702974e-15||>
# iter 2: <||1.31363359814332e-15||>
# iter 3: <||1.25607396694702e-15||>
# iter 4: <||1.25607396694702e-15||>
# iter 5: <||1.25607396694702e-15||>
# Tempo LU: 0.00495000183582306
# Tempo iter: 0.00668233260512352
# Tempo norma: 0.000754999617735545
3
0.0476190476190476 -0.0833333333333334 0.0357142857142857 
-0.952380952380952 1.41666666666667 -0.464285714285714 
4.57142857142857 -5 1.42857142857143
```

>As primeiras i linhas do arquivo contêm o valor da norma L2 em cada iteração do refinamento, lembrando que o número de iterações é indicado na execução do programa por meio do parâmetro -i.
>>**Tempos na saída do programa:** 
>>
>>`Tempo LU:` O tempo gasto pelo programa para realizar a fatoração LU da matriz de entrada.
>>
>>`Tempo iter:` O tempo médio gasto em cada iteração do reifinamento.
>>
>>`Tempo norma:` O tempo médio para o cálculo da norma  durante as iterações do refinamento.
> 
> A linha seguinte mostra o tamanho da matriz de saída que é o mesmo tamanho da entrada.
> Por fim, nas últimas N linhas da saída podemos encontrar a matriz inversa obtida e refinada.

### Sobre o código
  O programa possui dois módulos principais, sendo eles **invmat.c** e a pasta **/methods**. As principais funções destes módulos são: obter a inversa da matriz de entrada e aplicar o refinamento no resultado encontrado. O restante do código é composto de funções utilitárias que servem para realizar etapas específicas que são necessárias para chegar em uma resposta final além de funções para lidar com alocação de memória e tratamento de erros em ponto flutuante.
  Todas as funções estão documentadas no código, onde podemos encontrar uma breve descrição do que ela faz, além das descrições de cada parâmetro que recebe e de seu retorno.

### Funções principais 
  Como comentado anteriormente, a implementação das principais funções encontram-se na pasta **/methods**. Por esse motivo, essa seção é dedicada a explicar um pouco dessas funções.

  A pasta **/methods** possui dois arquivos: **methods.c** e **methods.h**. O **methods.c** possui a implementação das funções, enquanto o **methods.h** possui o protótipo das funções. O arquivo **methods.c** possui quatro funções: 

  >`refinement:` Essa função é responsável por realizar o refinamento da matriz inversa obtida como solução inicial, obtendo uma matriz inversa mais exata.

  >`gaussElimination:` Aplica a eliminação de gauss, com pivoteamento parcial, em uma matriz.

  >`factorizationLU:` Aplica a fatoração LU na matriz de entrada, utilizando a função `gaussElimination`.

  >`reverseMatrix:` Obtém a inversa da matriz de entrada, utilizando a `factorizationLU` para reaproveitar a eliminação gaussiana em cada sistema linear a ser resolvido.

### Lidando com erros 
  Sabemos que durante a execução do programa podem ocorrer exceções. Para lidarmos com isso, utilizamos um enum que enumera os possíveis status de execução das funções, que podem ser de sucesso ou de erro.

  ```c
  typedef enum
  {
    success = 0,      // sucesso
    infErr,           // Alguma operação aritmética gerou infinito
    nanErr,           // Alguma operação aritmética gerou NaN
    allocErr,         // Ocorreu algum erro durante alocação dinâmica de memória
    nonInvertibleErr, // A matriz de entrada não é invertível
    fileErr,          // Ocorreu algum erro ao abrir arquivos
    fileInputEmpty,   // O arquivo de entrada está vazio
    missingData,      // Os dados da matriz de entrada estão incompletos
  } FunctionStatus;
  ``` 
  Podemos notar que no código todas as funções capazes de gerar exceções possuem o retorno da função do tipo FunctionStatus. Essa foi uma forma que encontramos para contornar os erros na nossa **main()**, pois sempre fazemos verificações antes de avançar com a execução do programa.  

<p align="center">
    <strong>Desenvolvido por:</strong>
</p>
<p align="center">
    <b>Ariel Evaldt Schmitt</b> (GRR20203949) 
    <br/> 
    <b>Luis Felipe Risch</b> (GRR20203940)
</p>

