#!/bin/bash

# ==============================================================================
#
#  A "Guess the Number" CGI game written in Bash.
#  - No dependencies, uses standard shell commands.
#  - Manages state by passing it through hidden form fields.
#
# ==============================================================================

# --- Helper Functions ---

# urldecode: Decodes URL-encoded strings (e.g., "Hello+World" -> "Hello World")
urldecode() {
    # This uses sed to replace '+' with ' ' and '%' followed by hex codes with the actual characters.
    echo -e "$(sed 's/+/ /g;s/%\(..\)/\\x\1/g;')"
}

# obfuscate: A simple ROT13-like cipher to hide the number from the user.
obfuscate() {
    # It shifts characters by 13 positions. Applying it twice returns the original string.
    tr 'A-Za-z0-9' 'N-ZA-Mn-za-m4-90-3'
}

# --- Main CGI Logic ---

# 1. Parse the request body for POST requests.
if [ "$REQUEST_METHOD" = "POST" ]; then
    # Read the URL-encoded form data from stdin.
    read -r POST_DATA
    # Split the data into parameters.
    eval "$(echo "$POST_DATA" | urldecode | sed 's/&/;/g')"
fi

# 2. Set up game state.
# If 'secret' is not provided in the form data, it's a new game.
if [ -z "$secret" ]; then
    # New Game
    # Generate a random number between 1 and 100.
    SECRET_NUMBER=$(( (RANDOM % 100) + 1 ))
    # Obfuscate it for the hidden form field.
    OBFUSCATED_SECRET=$(echo "$SECRET_NUMBER" | obfuscate)
    ATTEMPTS=0
    MESSAGE="I have picked a number between 1 and 100. Can you guess it?"
else
    # Continue Game
    # De-obfuscate the secret number and get other state from the form data.
    SECRET_NUMBER=$(echo "$secret" | obfuscate)
    ATTEMPTS=$((attempts + 1))
    
    # Compare the guess with the secret number.
    if ! [[ "$guess" =~ ^[0-9]+$ ]]; then
         MESSAGE="That's not a number! Try again."
         ATTEMPTS=$((attempts - 1)) # Don't count an invalid guess
    elif [ "$guess" -lt "$SECRET_NUMBER" ]; then
        MESSAGE="Too low! Try a higher number."
    elif [ "$guess" -gt "$SECRET_NUMBER" ]; then
        MESSAGE="Too high! Try a lower number."
    else
        # The user guessed correctly!
        MESSAGE="Congratulations! You guessed the number <strong>$SECRET_NUMBER</strong> in $ATTEMPTS attempts!"
        GAME_OVER=true
    fi
    # Keep the same obfuscated secret for the next round.
    OBFUSCATED_SECRET="$secret"
fi

# 3. Print the HTTP Response (Headers + HTML Page)
# This is the output that your webserver will capture and send to the client.

# Start with the mandatory CGI headers and a blank line.
echo "Content-Type: text/html; charset=utf-8"
echo "Status: 200 OK"
echo ""

# Use a "here document" (cat <<EOF) to print the HTML page.
cat <<EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Bash Guessing Game</title>
    <style>
        body { font-family: 'Trebuchet MS', sans-serif; background: #282c34; color: #abb2bf; text-align: center; padding-top: 50px; }
        .game-container { background: #21252b; max-width: 500px; margin: auto; padding: 30px; border-radius: 15px; box-shadow: 0 5px 15px rgba(0,0,0,0.2); border: 1px solid #3a3f4c; }
        h1 { font-size: 2.5em; color: #61afef; }
        p { font-size: 1.2em; color: #e5c07b; min-height: 25px; }
        input[type="number"] { width: 100px; padding: 10px; border-radius: 5px; border: 1px solid #3a3f4c; background: #282c34; color: #abb2bf; text-align: center; font-size: 1.2em; }
        input[type="submit"] { background-color: #98c379; color: #21252b; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 1.2em; font-weight: bold; margin-left: 10px; transition: background-color 0.3s; }
        input[type="submit"]:hover { background-color: #a9d48a; }
        a { color: #c678dd; text-decoration: none; font-weight: bold; font-size: 1.2em; }
    </style>
</head>
<body>
    <div class="game-container">
        <h1>Bash Guessing Game</h1>
        <p>$MESSAGE</p>
EOF

# If the game is over, show a "Play Again" link. Otherwise, show the form.
if [ "$GAME_OVER" = true ]; then
cat <<EOF
        <p><a href="?reset=1">Play Again?</a></p>
EOF
else
cat <<EOF
        <form action="" method="post">
            <!-- Hidden fields to maintain state between requests -->
            <input type="hidden" name="secret" value="$OBFUSCATED_SECRET">
            <input type="hidden" name="attempts" value="$ATTEMPTS">
            
            <input type="number" name="guess" min="1" max="100" autofocus required>
            <input type="submit" value="Guess">
        </form>
EOF
fi

# Close the HTML structure.
cat <<EOF
    </div>
</body>
</html>
EOF
```

### How to Use

1.  **Save the file:** Save the code above as `guess_the_number.sh` in your CGI directory.
2.  **Make it executable:** Run the command `chmod +x guess_the_number.sh`.
3.  **Configure your webserv:** Make sure your server configuration correctly maps a route (e.g., `/cgi-bin/game`) to execute this script.
4.  **Access it:** Navigate to the corresponding URL in your browser, and the game will start.