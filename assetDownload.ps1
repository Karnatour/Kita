$texturesDir = "Engine/assets/textures"
$modelsDir   = "Engine/assets/models"

# Format: "save_as_filename" = "url"
# For archives (zip/rar), use the archive name as key — it will be
# extracted into the folder and the archive deleted afterwards.
$textures = @{
  "DefaultSkybox.hdr" = "https://dl.polyhaven.org/file/ph-assets/HDRIs/hdr/4k/kloofendal_48d_partly_cloudy_puresky_4k.hdr"
  # "my_name.png"      = "https://example.com/some_texture.png"
}

$models = @{
  "main-sponza.zip"  = "https://cdrdv2.intel.com/v1/dl/getContent/830833"
  "pkg_a_curtains.zip" = "https://cdrdv2.intel.com/v1/dl/getContent/726650"
}

function Download-File($url, $dest) {
  try {
    Write-Host "  Trying WebClient..."
    $wc = New-Object System.Net.WebClient
    $wc.DownloadFile($url, $dest)
  } catch {
    Write-Host "  WebClient failed, falling back to Invoke-WebRequest..."
    $ProgressPreference = 'SilentlyContinue'
    Invoke-WebRequest -Uri $url -OutFile $dest
  }
}

function Get-StemName($filename) {
  return [System.IO.Path]::GetFileNameWithoutExtension($filename)
}

function Download-Assets($dir, $files) {
  foreach ($filename in $files.Keys) {
    $dest = Join-Path $dir $filename
    $ext  = [System.IO.Path]::GetExtension($filename).ToLower()

    # For archives, check if extracted folder already exists.
    # For plain files, check if the file itself exists.
    $skip = switch ($ext) {
      ".zip" { Test-Path (Join-Path $dir (Get-StemName $filename)) }
      ".rar" { Test-Path (Join-Path $dir (Get-StemName $filename)) }
      default { Test-Path $dest }
    }

    if ($skip) {
      Write-Host "Skipping $filename (already exists)"
      continue
    }

    # Ensure destination directory exists
    if (-not (Test-Path $dir)) {
      New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }

    Write-Host "Downloading $filename..."
    Download-File $files[$filename] $dest

    switch ($ext) {
      ".zip" {
        Write-Host "Extracting $filename..."
        Expand-Archive -Path $dest -DestinationPath $dir -Force
        Remove-Item $dest
        Write-Host "Extracted and removed $filename"
      }
      ".rar" {
        Write-Host "Extracting $filename..."
        & unrar x -idq "$dest" "$dir\"
        Remove-Item $dest
        Write-Host "Extracted and removed $filename"
      }
      default {
        Write-Host "Saved to $dest"
      }
    }
  }
}

Download-Assets $texturesDir $textures
Download-Assets $modelsDir   $models

Write-Host "Done."