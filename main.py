from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.options import Options
import time

def setup_driver():
    chrome_options = Options()
    chrome_options.add_argument("--disable-gpu")
    chrome_options.add_argument("--disable-extensions")
    chrome_options.add_argument("--headless=new")
    return webdriver.Chrome(options=chrome_options)

def convertFileFormat(moves):
    new_moves=[]
    for move in moves:
        if("'" in move):
            new_moves.append("\n"+move[0]+"\n1")
        elif("2"in move):
            new_moves.append("\n"+move[0]+"\n0")
            new_moves.append("\n"+move[0]+"\n0")
        else:
            new_moves.append("\n"+move+"\n0")
    return new_moves
def convertFromFile(fileName):
    with open(fileName, "r") as f:
        lines = f.readlines()
        scramble =""
        for i in range(1,len(lines)) :
            line = lines[i].strip("\n")
            if(line=="0"):
                scramble+=" "
            elif(line=="1"):
                scramble+="' "
            else: 
                scramble+=line
        return scramble.strip()


def get_cube_solution(scramble):
    driver = setup_driver()
    
    try:
        # Navigate to the website
        url = "https://rubiks-cube-solver.com"
        driver.get(url)
        
        # Handle cookie consent
        try:
            agree_button = WebDriverWait(driver, 5).until(
                EC.element_to_be_clickable((By.CLASS_NAME, "css-47sehv"))
            )
            agree_button.click()
        except Exception as e:
            print("Cookie consent button not found or not needed:", e)
        
        # Input scramble
        scramble_input = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.ID, "scrambleAlg"))
        )
        scramble_input.clear()
        scramble_input.send_keys(scramble)
        
        # Execute scramble by clicking button
        original_window = driver.current_window_handle
        scramble_exe = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.ID, "executeScrambleAlg"))
        )
        scramble_exe.click()
        time.sleep(3)
        
        # Click solve button
        solve_btn = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.LINK_TEXT, "Solve"))
        )
        solve_btn.click()
        WebDriverWait(driver, 10).until(EC.number_of_windows_to_be(2))
        
        for window_handle in driver.window_handles:
            if window_handle != original_window:
                driver.switch_to.window(window_handle)
                break
        
        # Wait for the progress overlay to disappear
        print("Calculating Solution...")
        WebDriverWait(driver, 120).until(
            EC.invisibility_of_element_located((By.ID, "pleasewait"))
        )


        print("solution found... outputing to file")
        solution_div = WebDriverWait(driver, 10).until(
            EC.visibility_of_element_located((By.ID, "segedvaltozo"))
        )

        # Extract the moves from the solution
        moves = []
        spans = solution_div.find_elements(By.TAG_NAME, "span")
    
        if len(spans) == 0:
            print("No span elements found inside segedvaltozo. Check if the page layout changed.")
        else:
            for i in range(1,len(spans)):
                moves.append(spans[i].text)
            new_moves = convertFileFormat(moves)
            with open("output.txt","w") as f:
                f.write(str(len(new_moves)))
                for move in new_moves:
                    print(move)
                    f.write(move)
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        return None
        
    finally:
        driver.quit()

if __name__ == "__main__":
    # scramble = "U R2 F B R B2"
    scramble = convertFromFile("scramble.txt")
    solution = get_cube_solution(scramble)