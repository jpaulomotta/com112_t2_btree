def make_random_file(n)
  file = File.open("entrada_#{n/1000}k.txt", "w")
  file.puts(21)
  file.puts(n.times.map{|i| rand(10**9)}.join(", "))
end
