# DES-Encryptor-Decryptor
This code converts a 64-bit data into a DES cipher using a 64-bit key. Result cipher can be decrypted using the same code, you need only to add the option "decrypt" instead of "encrypt"

Also it calculates number of CPU Cycles taken to encipher this data

## Usage
./DES.exe encrypt data key

## Example:
   .\DES.exe encrypt 7A6C731D22347676 1323445A6D788381
output:
    cipher: 7DB033DD14D6F975
    Cycles: 11130
    
   .\DES.exe decrypt 7DB033DD14D6F975 1323445A6D788381
output:
    plain: 7A6C731D22347676
    Cycles: 9630
    
    
