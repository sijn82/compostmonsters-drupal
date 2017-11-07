<?php
// This template is used to tailor the add new monster incident page for users without full admin status.

module_load_include('inc', 'node', 'node.pages');

$form = node_add('monsters');
//print drupal_render($breadcrumb);
?>
<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
    <div class="<?php print $container_class; ?>">
        <div class="navbar-header">
            <?php if ($logo): ?>
                <a class="logo navbar-btn pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                </a>
            <?php endif; ?>

            <?php if (!empty($site_name)): ?>
                <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
            <?php endif; ?>

            <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navbar-collapse">
                    <span class="sr-only"><?php print t('Toggle navigation'); ?></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
            <?php endif; ?>
        </div>

        <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
            <div class="navbar-collapse collapse" id="navbar-collapse">
                <nav role="navigation">
                    <?php if (!empty($primary_nav)): ?>
                        <?php print render($primary_nav); ?>
                    <?php endif; ?>
                    <?php if (!empty($secondary_nav)): ?>
                        <?php print render($secondary_nav); ?>
                    <?php endif; ?>
                    <?php if (!empty($page['navigation'])): ?>
                        <?php print render($page['navigation']); ?>
                    <?php endif; ?>
                </nav>
            </div>
        <?php endif; ?>
    </div>
</header>
<!--</div>-->
    <header role="banner" id="page-header">
        <?php if (!empty($site_slogan)): ?>
            <p class="lead"><?php print $site_slogan; ?></p>
        <?php endif; ?>
        <?php if (!empty($title)): ?>
            <h1 class="page-header <?php if ($user->uid === 0) {print 'access-denied';} ?>"><?php print $title; ?></h1>
        <?php endif; ?>
    </header> <!-- /#page-header -->
<!--</header>-->

<div class="main-container add_new_monsters <?php print $container_class; ?>">

    <div class="row">
    <?php
    if($user->uid !== 0) {print drupal_render($form);}

//    print drupal_render($form['title']);
//    print drupal_render($form['field_monster_image']);
//    print drupal_render($form['body']);
//    print drupal_render($form['monster_witnessed_by']);
//    print drupal_render($form['field_location_spotted']);
    ?>
    </div>
    <?php if (!empty($page['footer'])): ?>
        <footer class="footer col-sm-10 <?php
        if($user->uid !== 0) {print 'display-none ';}
        print $container_class; ?>">
            <div>
                <div class="col-sm-5 col-sm-offset-1" >
                    <?php print render($page['footer']['user_login']); ?>
                </div>
                <div class="col-sm-5">
                    <?php print render($page['footer']['views_about-block_1']); ?>
                </div>
        </footer>
    <?php endif; ?>
</div>

