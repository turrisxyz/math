kernel void calculate(__global bool* var1_global, int var1_rows, int var1_view, __global double* var2_global, int var2_rows, int var2_view, __global double* var3_global, int var3_rows, int var3_view, __global double* var5_global, int var5_rows, int var5_view){
int i = get_global_id(0);
int j = get_global_id(1);
bool var1 = 0; if (!((!contains_nonzero(var1_view, LOWER) && j < i) || (!contains_nonzero(var1_view, UPPER) && j > i))) {var1 = var1_global[i + var1_rows * j];}
double var2 = 0; if (!((!contains_nonzero(var2_view, LOWER) && j < i) || (!contains_nonzero(var2_view, UPPER) && j > i))) {var2 = var2_global[i + var2_rows * j];}
double var3 = 0; if (!((!contains_nonzero(var3_view, LOWER) && j < i) || (!contains_nonzero(var3_view, UPPER) && j > i))) {var3 = var3_global[i + var3_rows * j];}
double var4 = var1 ? var2 : var3;
var5_global[i + var5_rows * j] = var4;
}