# intended to improve output format for emacs compilation buffer
# call compile command with 'M-x compile' and pipe norminette output into awk 
# norminette | awk -f normformat
# you will be able to use M-n and M-p to navigate errors C-o to look error on
# other buffer and RET to visit error on other buffer

/OK!$/ { print $0 }

/Error!$/ { file_name=$1 }

/^Error/ {
	line = $4;
	sub(/,/,"",line);
	col = 0;
	sub(/\):/, "", col);
	print file_name line ":" col, $0
}
