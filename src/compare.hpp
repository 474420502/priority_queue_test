#ifndef __COMPARE__
#define __COMPARE__

template<class T>
int compare(T v1, T v2)
{
  if (v1 > v2)
  {
    return 1;
  }
  else if (v1 < v2)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

#endif // __COMPARE__