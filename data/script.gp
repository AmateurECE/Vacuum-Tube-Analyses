# f(x,y) = a*x + b*y + c*x**2 + d*y**2 + e*x**3 + f*y**3 + g*x**4 + h*y**4 + i
f(x,y) = a*x + b*y + c*x**2 + d*y**2 + e
# fit [:] f(x,y) '12AX7-Data.txt' using 1:2:3 via a,b,c,d,e,f,g,h,i
fit [:] f(x,y) '12AX7-Data.txt' using 1:2:3 via a,b,c,d,e
set zrange [-1:8]
splot '12AX7-Data.txt' using 1:2:3, f(x,y)
pause -1
