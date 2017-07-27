<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
?>  
<div class="menu_overlay"></div> 
<div id='collapsable' class='expanded menu_wrapper'><div class="explore_menu">Explore</div>
    <div id="block-<?php print $block->module . '-' . $block->delta; ?>" class="menu_dropdown <?php print $classes; ?> " <?php print $attributes; ?> >
        <?php print render($title_prefix); ?>
        <?php if ($block->subject): ?>
          <h2><?php print $block->subject ?></h2>
        <?php endif; ?>
        <?php print render($title_suffix); ?>

        <div class="content"<?php print $content_attributes; ?>>
            <?php print $content ?>
        </div>
    </div>
</div>