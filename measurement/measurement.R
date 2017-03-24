deg = c(-50,-40,-30,-20,-10,0,10,20,30,40,50)
count = c(-3281,-2684,-1743,-1171,-719,152,808,1145,2119,2817,3728)

countDeg = lm(count ~ deg)
degCount = lm(deg~count)



summary(countDeg)
summary(degCount)

plot(deg, count, pch = 19)
abline(countDeg, col = "red")


plot(count, deg, pch = 19)
abline(degCount, col = "red")

predict(degCount, data.frame(count=2000))

