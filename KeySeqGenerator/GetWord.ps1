  $tfn = $fn = $args[0] -replace ".txt", ''
  $fn = $fn + ".txt"
  $tfn = $tfn + "_tmp.txt"

  Get-Content $fn | %{$_ -replace '[\(\)\-\`*:".,?!#]+',' ' -replace "[']+",''} | %{$_ -replace ' ', "`n"} | Out-File $tfn
  Get-Content $tfn | Sort-Object -Unique | Out-File $fn
  rm $tfn
