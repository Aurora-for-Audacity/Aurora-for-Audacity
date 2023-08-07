# -----------------------------------------------------------------------------
# Update internal paths in order to use the libraries stored inside Audacity bundle

# --- functions ---------------------------------------------------------------

replace_pattern()
{
    # /opt/local/lib -> @rpath
    FN=$1   #12345678901234
    PATTERN="/opt/local/lib"
    
    echo "Processing file $FN...[$WX_VER] -> [$WX_FULL_VER]"
    
    otool -L "$FN" | awk '{ print $1 }' | while read DEP
    do
        if [ ${DEP:0:14} == "$PATTERN" ]
        then
            LIB=$(echo $FN  | awk -F"/" '{ print $NF }')
            LIB=$(echo $LIB | awk -F"." '{ print $1 }')
            
            if [ "$DEP" != "${DEP/$LIB/}" ]
            then
                MOD_DEP=${DEP/\/opt\/local\/lib/@executable_path\/..\/Frameworks}
                MOD_DEP=${MOD_DEP/$WX_VER/$WX_FULL_VER}  # I don't know exactly why this is needed...
                
                #if [ $CONFIGURATION == "Release" ]
                #then
                    # that's weird...
                #    MOD_DEP=${MOD_DEP/_baseu_/_baseu_release}
                #    MOD_DEP=${MOD_DEP/_cocoau_/_cocoau_release_}
                #fi
                echo "id: $DEP -> $MOD_DEP"
                install_name_tool -id $MOD_DEP "$FN"
            else
                MOD_DEP=${DEP/\/opt\/local\/lib/@executable_path\/..\/Frameworks}
                MOD_DEP=${MOD_DEP/$WX_VER/$WX_FULL_VER}  # I don't know exactly why this is needed...
                
                #if [ $CONFIGURATION == "Release" ]
                #then
                    # that's weird...
                #    MOD_DEP=${MOD_DEP/_baseu/_baseu_release}
                #    MOD_DEP=${MOD_DEP/_cocoau_/_cocoau_release_}
                #fi
                echo "dep: $DEP -> $MOD_DEP"
                install_name_tool -change $DEP $MOD_DEP "$FN"
            fi
        fi
    done
}

# --- main --------------------------------------------------------------------
CURRENT_DIR=$(pwd)

cd "$TARGET_BUILD_DIR"

replace_pattern $EXECUTABLE_NAME

cd "$CURRENT_DIR"
