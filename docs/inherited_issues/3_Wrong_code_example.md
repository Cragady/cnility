# Issue 3

## N-R-K opened on Jun 8, 2021 · edited by N-R-K

Page 75, right above exercise 3-3.

```c
for (i = 0, j = strlen(s)−1; i < j; i++, j--)
   c = s[i]; s[i] = s[j]; s[j] = c;
```
these should be commas, not semicolon.

## N-R-K on Nov 14, 2021 · edited by N-R-K


The qsort function on page 132 is wrong.

Also one more error on page 133

> The swap function, which exchanges two pointers, is identical to what we
> presented earlier in the chapter, except that the declarations are changed to
> void *.

```c
/* swap: interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
    char *temp;
    
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
```


