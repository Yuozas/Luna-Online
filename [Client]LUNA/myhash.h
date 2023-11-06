#import advapi32.dll

Private Declare Function CryptAcquireContext Lib "advapi32.dll" _
Alias "CryptAcquireContextA" ( _
ByRef phProv As Long, _
ByVal pszContainer As String, _
ByVal pszProvider As String, _
ByVal dwProvType As Long, _
ByVal dwFlags As Long) As Long

Private Declare Function CryptReleaseContext Lib "advapi32.dll" ( _
ByVal hProv As Long, _
ByVal dwFlags As Long) As Long

Private Declare Function CryptCreateHash Lib "advapi32.dll" ( _
ByVal hProv As Long, _
ByVal Algid As Long, _
ByVal hKey As Long, _
ByVal dwFlags As Long, _
ByRef phHash As Long) As Long

Private Declare Function CryptDestroyHash Lib "advapi32.dll" ( _
ByVal hHash As Long) As Long

Private Declare Function CryptHashData Lib "advapi32.dll" ( _
ByVal hHash As Long, _
pbData As Any, _
ByVal dwDataLen As Long, _
ByVal dwFlags As Long) As Long

Private Declare Function CryptGetHashParam Lib "advapi32.dll" ( _
ByVal hHash As Long, _
ByVal dwParam As Long, _
pbData As Any, _
pdwDataLen As Long, _
ByVal dwFlags As Long) As Long

Private Const PROV_RSA_FULL = 1

Private Const CRYPT_NEWKEYSET = &H8

Private Const ALG_CLASS_HASH = 32768

Private Const ALG_TYPE_ANY = 0

Private Const ALG_SID_MD2 = 1
Private Const ALG_SID_MD4 = 2
Private Const ALG_SID_MD5 = 3
Private Const ALG_SID_SHA1 = 4

Enum HashAlgorithm
MD2 = ALG_CLASS_HASH Or ALG_TYPE_ANY Or ALG_SID_MD2
MD4 = ALG_CLASS_HASH Or ALG_TYPE_ANY Or ALG_SID_MD4
md5 = ALG_CLASS_HASH Or ALG_TYPE_ANY Or ALG_SID_MD5
SHA1 = ALG_CLASS_HASH Or ALG_TYPE_ANY Or ALG_SID_SHA1
End Enum

 Const int HP_HASHVAL = 2
 Const HP_HASHSIZE = 4

Function HashFile( _
ByVal Filename As String, _
Optional ByVal Algorithm As HashAlgorithm = md5) As String
Dim hCtx As Long
Dim hHash As Long
Dim lFile As Long
Dim lRes As Long
Dim lLen As Long
Dim lIdx As Long
Dim abHash() As Byte
If Len(Dir$(Filename)) = 0 Then Err.Raise 53
lRes = CryptAcquireContext(hCtx, vbNullString, _
vbNullString, PROV_RSA_FULL, 0)

If lRes = 0 And Err.LastDllError = &H80090016 Then
lRes = CryptAcquireContext(hCtx, vbNullString, _
vbNullString, PROV_RSA_FULL, CRYPT_NEWKEYSET)
End If

If lRes <> 0 Then
lRes = CryptCreateHash(hCtx, Algorithm, 0, 0, hHash)

If lRes <> 0 Then
lFile = FreeFile
Open Filename For Binary As lFile

If Err.Number = 0 Then

Const BLOCK_SIZE As Long = 32 * 1024& ' 32K
ReDim abBlock(1 To BLOCK_SIZE) As Byte
Dim lCount As Long
Dim lBlocks As Long
Dim lLastBlock As Long
lBlocks = LOF(lFile) \ BLOCK_SIZE
lLastBlock = LOF(lFile) - lBlocks * BLOCK_SIZE
For lCount = 1 To lBlocks
Get lFile, , abBlock
lRes = CryptHashData(hHash, abBlock(1), BLOCK_SIZE, 0)
If lRes = 0 Then Exit For

Next
If lLastBlock > 0 And lRes <> 0 Then
ReDim abBlock(1 To lLastBlock) As Byte
Get lFile, , abBlock
lRes = CryptHashData(hHash, abBlock(1), lLastBlock, 0)

End If
Close lFile

End If

If lRes <> 0 Then
lRes = CryptGetHashParam(hHash, HP_HASHSIZE, lLen, 4, 0)

If lRes <> 0 Then
ReDim abHash(0 To lLen - 1)
lRes = CryptGetHashParam(hHash, HP_HASHVAL, abHash(0), lLen, 0)
If lRes <> 0 Then
For lIdx = 0 To UBound(abHash)
HashFile = HashFile & _
Right$("0" & Hex$(abHash(lIdx)), 2)
Next

End If

End If

End If
CryptDestroyHash hHash

End If

End If
CryptReleaseContext hCtx, 0
If lRes = 0 Then Err.Raise Err.LastDllError
End Function
