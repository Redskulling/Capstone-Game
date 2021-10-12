while(1) do
io.write("Dex Number: ");
x=io.read("*n");
y=(x%30)
x=(x/30)+1
x=math.floor(x);
io.write("Box: " .. x .. "\nRow: " .. y%6 .. "\nCollumn: " .. math.floor((y/6))+1 .. "\n");
end
