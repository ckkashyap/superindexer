$str="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
$length=$str.length
def getRandomWord
	wordLength=3+rand(10)
	word=""
	wordLength.times do
		x=rand($length)
		y=$str[x,1]
		word.concat(y)
	end

	return word
end


5000.times do |i|
	puts getRandomWord
	puts getRandomWord
end

puts "ENDOFINPUT"
puts "ENDOFINPUT"

