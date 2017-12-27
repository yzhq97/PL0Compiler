var x, sum;
begin
    sum := 0;
    repeat
        read (x);
        sum := sum + x
    until x = 0;
    write(sum);
end.