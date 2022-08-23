/**********************************
/* Created by boil on 2022/8/14.
/**********************************

/*检查堆栈方向(0-down, 1-up) */
int f(int *a)
{
  int b;
  return(&b > a)?1:0;
}
/*通过指针调用函数来防止编译器优化。*/
volatile int (*ptr_f)(int *) = f;
int main()
{
  int a;
  return ptr_f(&a);
}