with open("test2.dat", "w") as f:
    for i in range(1200):
        f.write("%s,1"%i)
        for j in range(512):
            f.write(":%s,%s"%(j,j))
        f.write("\n")


