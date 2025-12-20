#!/bin/bash

# 1. Strict Error Handling
set -e
set -u

# 2. Silence all error messages
exec 2>/dev/null

# 3. Critical Variable Validation (Silent)
: "$REQUEST_METHOD"

# --- Function to print the main game form ---
print_game_form() {
    local MESSAGE="$1"
    local SECRET_NUM="$2"
    local GUESS_COUNT="$3"

    cat << EOF
    <h1>Guess The Number</h1>
    <p>Current Guesses: <span class="output">$GUESS_COUNT</span></p>
    <h2>$MESSAGE</h2>
    
    <form method="POST" action="guess_game.sh">
        <input type="hidden" name="secret" value="$SECRET_NUM">
        <input type="hidden" name="guesses" value="$GUESS_COUNT">
        
        <label for="guess">Your Guess:</label>
        <input type="number" id="guess" name="guess" min="1" max="100" required>
        <button type="submit">Submit Guess</button>
    </form>
EOF
}

# --- Function to print the win page ---
print_win_page() {
    local SECRET_NUM="$1"
    local FINAL_COUNT="$2"

    cat << EOF
    <h1>You Guessed It!</h1>
    <pre>
<span class="prompt">$</span> <span class="output">Target acquired: $SECRET_NUM</span>
<span class="prompt">$</span> <span class="output">Attempts logged: $FINAL_COUNT</span>
<span class="prompt">$</span> <span class="output">System... compromised.</span>
    </pre>
    <a href="guess_game.sh" class="play-again">Play Again</a>
EOF
}

# --- Start HTML Output (Only reached if REQUEST_METHOD exists) ---
cat << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Guess Game</title>
    <style>
        body { background-color: #000; color: #39ff14; font-family: monospace; padding: 2rem; text-align: center; }
        h1, h2 { text-shadow: 0 0 5px #39ff14; }
        h1 { border-bottom: 2px solid #39ff14; padding-bottom: 5px; }
        pre { background-color: #0a0a0a; border: 1px solid #39ff14; padding: 1rem; border-radius: 8px; margin: 2rem auto; text-align: left; max-width: 500px; }
        form { background-color: #0a0a0a; border: 1px solid #39ff14; padding: 2rem; max-width: 400px; margin: 1rem auto; }
        input, button { background-color: #111; border: 1px solid #39ff14; color: #39ff14; padding: 0.75rem; width: 90%; margin-bottom: 1rem; }
        button, .play-again { background-color: #39ff14; color: #000; cursor: pointer; font-weight: bold; text-decoration: none; padding: 0.75rem 1.5rem; display: inline-block; }
        .prompt { color: #ffffff; }
        .output { color: #39ff14; }
    </style>
</head>
<body>
EOF

# --- Main Logic ---

if [ "$REQUEST_METHOD" = "POST" ]; then
    # Read raw form data (fails if CONTENT_LENGTH is missing/incorrect due to set -e)
    read -n "${CONTENT_LENGTH:-0}" POST_DATA
    
    # Parse data
    USER_GUESS=$(echo "$POST_DATA" | tr '&' '\n' | grep '^guess=' | cut -d'=' -f2 || echo "")
    SECRET_NUM=$(echo "$POST_DATA" | tr '&' '\n' | grep '^secret=' | cut -d'=' -f2 || echo "")
    GUESS_COUNT=$(echo "$POST_DATA" | tr '&' '\n' | grep '^guesses=' | cut -d'=' -f2 || echo "0")

    GUESS_COUNT=$((GUESS_COUNT + 1))

    if ! [[ "$USER_GUESS" =~ ^[0-9]+$ ]]; then
        print_game_form "Invalid input." "$SECRET_NUM" "$GUESS_COUNT"
    elif [ "$USER_GUESS" -lt "$SECRET_NUM" ]; then
        print_game_form "Target is higher." "$SECRET_NUM" "$GUESS_COUNT"
    elif [ "$USER_GUESS" -gt "$SECRET_NUM" ]; then
        print_game_form "Target is lower." "$SECRET_NUM" "$GUESS_COUNT"
    else
        print_win_page "$SECRET_NUM" "$GUESS_COUNT"
    fi

elif [ "$REQUEST_METHOD" = "GET" ]; then
    SECRET_NUM=$(( ( RANDOM % 100 ) + 1 ))
    GUESS_COUNT=0
    print_game_form "New target acquired. Guess (1-100)." "$SECRET_NUM" "$GUESS_COUNT"
fi

cat << EOF
</body>
</html>
EOF