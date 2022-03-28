# fcd
Fast Change Directory.
Fuzzy search through a list of your most used files and directories. Selecting a file will edit it, while selecting a directory will CD to it. I find this useful since 99% of the time I am navigating to the same directories and files at work.

<img width="603" alt="fcd" src="https://user-images.githubusercontent.com/83528263/160432459-ec56d423-1982-435d-8332-732189eaa9da.png">

## zsh setup

```
fcd() {
    out="$HOME/.cache/fcd_dest_this_file_will_be_deleted"
    echo "" > "$out"
    ~/.bin/fcd ~/.config/fcd/directories.txt "$out"
    if [ -e "$out" ]; then
        file=$(cat "$out")

        file=${file//\~/$HOME}
        file=${file//\$HOME/$HOME}

        if [ -e "$file" ]; then
            if [ -d "$file" ]; then
                cd "$file"; ls
            else
                vim "$file"
            fi
        fi
    fi
}
```

This adds the logic of opening a file and changing directory.
It's a zsh function because we want to change directory after the program ends.
