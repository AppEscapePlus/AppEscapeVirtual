# Variables, dirs and stuff
ARM_TOOLCHAIN="${HOME}/toolchains/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-"
CLANG="${HOME}/toolchains/linux-x86/clang-4691093/bin"
GCC="${HOME}/toolchains/aarch64-linux-android-4.9/bin"
KERNEL_DIR="${HOME}/whyred"
DEFCONFIG="whyred-perf_defconfig"
COLOR='\033[0;33m' #Yellow
ANYKERNEL_DIR="${HOME}/AnyKernel3"
KERNEL_NAME="IceColdR5.4Stable(NO-HALLSENSOR)"
DEVICE="_whyred-"   
ZIP_DATE=`date +%d-%m`
FINAL_ZIP="$KERNEL_NAME""$DEVICE""$ZIP_DATE".zip
 
 
echo -e "Starting...\n"
echo " "
 
 
#export CROSS_COMPILE_ARM32="$ARM_TOOLCHAIN/bin/arm-linux-androideabi-"
 
cd $KERNEL_DIR
make O=out ARCH=arm64 ${DEFCONFIG}
 
START=$(date +"%s")
 
PATH="$CLANG:$GCC:${PATH}" \
make -j$(nproc --all) O=out \
                      ARCH=arm64 \
                      CC=clang \
                      CLANG_TRIPLE=aarch64-linux-gnu- \
                      CROSS_COMPILE=aarch64-linux-android- \
              CROSS_COMPILE_ARM32="${ARM_TOOLCHAIN}"
 
if [[ $? -eq 1 ]]; then
    echo -e "${COLOR}Build failed :P"
    exit
else
    END=$(date +"%s")
    DIF=$((END - START))
    echo " "
    echo -e "Kernel build finished in $((DIF / 60)) minute(s) and $((DIF % 60)) seconds."
    echo " "
fi    
echo "Creating the zip..."
cd $ANYKERNEL_DIR
#clean old dtb, zips and mdf5
rm -rf *.zip
rm -rf Image.gz-dtb
rm -rf *.md5sum
#pull the dtb to anykernel dir
if [[ -f $KERNEL_DIR/arch/arm64/boot/Image.gz-dtb ]]; then
    cp $KERNEL_DIR/arch/arm64/boot/Image.gz-dtb $ANYKERNEL_DIR
elif [[ -f $KERNEL_DIR/out/arch/arm64/boot/Image.gz-dtb ]]; then
    echo -e "Image.gz-dtb found in /out\n"
    cp $KERNEL_DIR/out/arch/arm64/boot/Image.gz-dtb $ANYKERNEL_DIR
else
    echo -e "${COLOR}Image.gz-dtb not found, rip\n"
    exit
fi
 
    
START_ZIP=$(date +"%s")
zip -r9 $FINAL_ZIP * -x .git README.md
md5sum "${FINAL_ZIP}" | sed -e "s|${ANYKERNEL_DIR}||" > "${FINAL_ZIP}.md5sum"
END_ZIP=$(date +"%s")
DIFF=$((END_ZIP - START_ZIP))
echo -e "Zip created in $((DIFF / 60)) minute(s) and $((DIFF % 60)) seconds.\n"
echo " "
